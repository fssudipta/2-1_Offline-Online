create table job_rank (
   job_id varchar2(10),
   rank   number
);
create or replace procedure rank_jobs (
   min_hired_count in number
) is
   cursor cur_jobs is
   select j.job_id,
          j.job_title,
          count(e.employee_id) as hire_count,
          round(
             avg(e.salary),
             2
          ) as avg_salary
     from jobs j
     join employees e
   on j.job_id = e.job_id
    group by j.job_id,
             j.job_title
   having count(e.employee_id) >= min_hired_count;

   v_extracted_id varchar2(10);
   v_rank         number := 0;
   v_prev_avg     number := null;
begin
   for job_rec in cur_jobs loop
        -- Extract ID from JOB_TITLE
      declare
         v_first_word  varchar2(50);
         v_second_word varchar2(50);
         v_pos         number;
      begin
         v_pos := instr(
            job_rec.job_title,
            ' '
         );
         if v_pos > 0 then
            v_first_word := substr(
               job_rec.job_title,
               1,
               v_pos - 1
            );
            v_second_word := substr(
               job_rec.job_title,
               v_pos + 1
            );
         else
            v_first_word := job_rec.job_title;
            v_second_word := '';
         end if;

         v_extracted_id := upper(substr(
            v_first_word,
            1,
            2
         ))
                           || '_'
                           || upper(substr(
            v_second_word,
            1,
            3
         ));
      end;

        -- Check if JOB_ID matches extracted ID
      if job_rec.job_id = v_extracted_id then
         v_rank := v_rank + 1;
         insert into job_rank (
            job_id,
            rank
         ) values ( job_rec.job_id,
                    v_rank );
      end if;
   end loop;

   dbms_output.put_line('Jobs ranked and inserted into JOB_RANK table.');
exception
   when others then
      dbms_output.put_line('Error occurred: ' || sqlerrm);
end;
/

-- Clear previous runs
delete from job_rank;

-- Run the procedure
begin
   rank_jobs(min_hired_count => 5);
end;
/

-- View result
select *
  from job_rank
 order by rank;

-------------------------2--------------------------------
drop table temp_employees;
create table temp_employees
   as
      select *
        from employees;

create or replace trigger validate_salary_update before
   update of salary on temp_employees
   for each row
declare
   v_min_job_salary  number;
   v_min_mgr_salary  number;
   v_dept_mgr_salary number;
begin
    -- Rule 1: Salary cannot be less than min salary for same JOB_ID
   select min(salary)
     into v_min_job_salary
     from employees
    where job_id = :new.job_id;

   if :new.salary < v_min_job_salary then
      raise_application_error(
         -20001,
         'Salary cannot be below minimum for this job.'
      );
   end if;

    -- Rule 2: If NOT a manager → salary must be less than min salary of all managers
   if :new.job_id not in (
      select distinct job_id
        from employees
       where manager_id is null
   ) then
      select min(salary)
        into v_min_mgr_salary
        from employees
       where job_id in (
         select distinct job_id
           from employees
          where manager_id is null
      );

      if :new.salary >= v_min_mgr_salary then
         raise_application_error(
            -20002,
            'Non-manager salary must be less than minimum manager salary.'
         );
      end if;
   end if;

    -- Rule 3: If employee is a manager → salary must be less than department manager's salary
   if exists (
      select 1
        from employees
       where employee_id = :new.employee_id
         and employee_id in (
         select manager_id
           from employees
          where manager_id is not null
      )
   ) then
      select salary
        into v_dept_mgr_salary
        from employees
       where employee_id = (
         select manager_id
           from employees
          where department_id = (
               select department_id
                 from employees
                where employee_id = :new.employee_id
            )
            and manager_id is null
      );

      if :new.salary >= v_dept_mgr_salary then
         raise_application_error(
            -20003,
            'Manager salary must be less than department head.'
         );
      end if;
   end if;

exception
   when no_data_found then
      raise_application_error(
         -20004,
         'Required salary data not found.'
      );
   when others then
      raise_application_error(
         -20005,
         'Update denied: ' || sqlerrm
      );
end;
/

-- Valid test
update temp_employees
   set
   salary = 8000
 where employee_id = 178;

-- Should fail due to Rule 1
update temp_employees
   set
   salary = 3000
 where employee_id = 178;

-- Should fail due to Rule 2
update temp_employees
   set
   salary = 20000
 where employee_id = 198;

-- Should fail due to Rule 3
update temp_employees
   set
   salary = 20000
 where employee_id = 200;