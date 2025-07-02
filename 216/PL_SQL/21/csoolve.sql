-- Step 1: Create and populate TEMP_EMPLOYEES
create table temp_employees (
   employee_id  varchar2(10),
   name         varchar2(100),
   email        varchar2(50),
   manager_id   number(4,0),
   manager_cred varchar2(20)
);

insert into temp_employees (
   employee_id,
   name,
   email,
   manager_id
)
   select employee_id,
          first_name || last_name,
          email,
          manager_id
     from employees;

-- Step 2: Create the procedure
create or replace procedure populate_creds (
   min_emp_count in number,
   min_job_count in number
) is
   cursor cur_managers is
   select distinct manager_id
     from temp_employees
    where manager_id is not null;

   v_job_count number;
   v_emp_count number;
   v_email     varchar2(50);
   v_cred      varchar2(20);
begin
   for mgr_rec in cur_managers loop
        -- Count number of jobs the manager has worked in (from job_history + current)
      begin
         select count(distinct job_id)
           into v_job_count
           from (
            select job_id
              from job_history
             where employee_id = mgr_rec.manager_id
            union
            select job_id
              from employees
             where employee_id = mgr_rec.manager_id
         );
      exception
         when no_data_found then
            v_job_count := 0;
      end;

        -- Count number of employees directly managed
      select count(*)
        into v_emp_count
        from employees
       where manager_id = mgr_rec.manager_id;

        -- Check eligibility
      if v_job_count >= min_job_count
      or v_emp_count >= min_emp_count then
            -- Get manager's email
         select email
           into v_email
           from employees
          where employee_id = mgr_rec.manager_id;

            -- Build CREDENTIAL
         v_cred := substr(
            v_email,
            1,
            2
         )
                   || '**'
                   || substr(
            v_email,
            greatest(
                    length(v_email) - 1,
                    1
                 )
         )
                   || '-'
                   || v_emp_count;

            -- Update record
         update temp_employees
            set
            manager_cred = v_cred
          where employee_id = mgr_rec.manager_id;
      end if;
   end loop;
end;
/

begin
   populate_creds(
      min_emp_count => 5,
      min_job_count => 3
   );
end;
/

-- View results
select *
  from temp_employees
 where manager_cred is not null;

-----------------------2nd one------------------------
create table department_job_map_table
   as
      select distinct job_id,
                      department_id
        from employees
       where department_id is not null;

create or replace trigger validate_and_update_job before
   update of job_id on temp_employees
   for each row
declare
   v_old_max_salary number;
   v_new_max_salary number;
   v_hire_date      date;
   v_years_worked   number;
   v_new_dept_id    number;
begin
    -- Get max salary of old and new job
   select max(salary)
     into v_old_max_salary
     from employees
    where job_id = :old.job_id;

   select max(salary)
     into v_new_max_salary
     from employees
    where job_id = :new.job_id;

    -- Case 1: New job has higher rank → allow update
   if v_new_max_salary > v_old_max_salary then
        -- Get department ID from map
      select department_id
        into v_new_dept_id
        from department_job_map_table
       where job_id = :new.job_id;

      :new.department_id := v_new_dept_id;

    -- Case 2: New job has lower rank
   elsif v_new_max_salary < v_old_max_salary then
        -- Only allowed if employee has less than 5 years of experience
      select hire_date
        into v_hire_date
        from employees
       where employee_id = :new.employee_id;

      v_years_worked := months_between(
         sysdate,
         v_hire_date
      ) / 12;
      if v_years_worked >= 5 then
         raise_application_error(
            -20001,
            'Job downgrade not allowed: Employee has 5+ years of experience'
         );
      else
            -- Allow update, set new department ID
         select department_id
           into v_new_dept_id
           from department_job_map_table
          where job_id = :new.job_id;

         :new.department_id := v_new_dept_id;
      end if;

    -- Case 3: Same rank → do nothing
   else
      null;
   end if;

exception
   when no_data_found then
      raise_application_error(
         -20002,
         'Invalid JOB_ID or HIRE_DATE missing'
      );
   when others then
      raise_application_error(
         -20003,
         'Update failed: ' || sqlerrm
      );
end;
/

-- Test valid update
update temp_employees
   set
   job_id = 'SA_REP'
 where employee_id = 178;

-- Test invalid update (more than 5 years)
update temp_employees
   set
   job_id = 'AD_ASST'
 where employee_id = 101;