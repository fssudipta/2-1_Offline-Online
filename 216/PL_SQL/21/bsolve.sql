create table lowest_paid_employees (
   employee_id   varchar2(100),
   department_id varchar2(100),
   saldiff       number,
   bonus         number,
   tag           varchar2(100)
);

create or replace procedure update_lowest_paid_employees (
   p_m number,
   p_n number
) is
   cursor cur_departments is
   select distinct department_id
     from employees
    where department_id is not null;

   v_min_salary   number;
   v_max_salary   number;
   v_employee_id  employees.employee_id%type;
   v_saldiff      number;
   v_bonus        number := 0;
   v_years_worked number;
begin
    -- Clear existing data
   delete from lowest_paid_employees;

   for dept_rec in cur_departments loop
        -- Get min and max salary in department
      select min(salary),
             max(salary)
        into
         v_min_salary,
         v_max_salary
        from employees
       where department_id = dept_rec.department_id
         and hire_date <= add_months(
         sysdate,
         -12
      ); -- permanent employees only

        -- Skip if all have same salary
      if v_min_salary != v_max_salary then
            -- Find the lowest paid employee (oldest if tie)
         begin
            select employee_id,
                   salary
              into
               v_employee_id,
               v_saldiff
              from (
               select employee_id,
                      salary
                 from employees
                where department_id = dept_rec.department_id
                  and salary = v_min_salary
                  and hire_date <= add_months(
                  sysdate,
                  -12
               )
                order by hire_date asc
            )
             where rownum = 1;

                -- Compute saldiff
            v_saldiff := v_max_salary - v_saldiff;

                -- Compute years worked
            select round((sysdate - hire_date) / 365)
              into v_years_worked
              from employees
             where employee_id = v_employee_id;

                -- Base bonus
            v_bonus := p_m * floor(v_years_worked);

                -- Add management bonus
            declare
               v_sub_count number;
            begin
               select count(*)
                 into v_sub_count
                 from employees
                where manager_id = v_employee_id;

               if v_sub_count >= 5 then
                  select job_id
                    into v_employee_id
                    from employees
                   where employee_id = v_employee_id;

                  v_bonus := v_bonus + p_n;
               end if;
            end;

                -- Insert record
            insert into lowest_paid_employees (
               employee_id,
               department_id,
               saldiff,
               bonus,
               tag
            ) values ( v_employee_id,
                       dept_rec.department_id,
                       v_saldiff,
                       v_bonus,
                       null );
         end;
      end if;
   end loop;
end;
/

-------------2nd---------------
create or replace trigger update_lpet_tag_trigger before
   insert on lowest_paid_employees
   for each row
declare
   v_global_max_saldiff number;
begin
    -- Get global max saldiff
   select max(saldiff)
     into v_global_max_saldiff
     from lowest_paid_employees;

    -- Sanity check
   if :new.saldiff > v_global_max_saldiff then
      raise_application_error(
         -20001,
         'Invalid saldiff: Exceeds global maximum.'
      );
   end if;

    -- Set tag
   if :new.saldiff < 10000 then
      :new.tag := 'low';
   elsif :new.saldiff < 20000 then
      :new.tag := 'very low';
   else
      :new.tag := 'extremely low';
   end if;
end;
/

begin
   update_lowest_paid_employees(
      10000,
      5000
   );
end;
/

-- View results
select *
  from lowest_paid_employees;