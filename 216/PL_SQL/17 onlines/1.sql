create table emp_temp (
   employee_id    varchar2(10),
   name           varchar2(100),
   salary         number(10,0),
   commission_pct number(10,2),
   manager_id     number(4,0),
   department_id  number(4,0)
);
insert into hr.emp_temp (
   employee_id,
   name,
   salary,
   commission_pct,
   manager_id,
   department_id
)
   select employee_id,
          first_name
          || ' '
          || last_name,
          salary,
          commission_pct,
          manager_id,
          department_id
     from hr.employees;

create or replace function defaultcommissionpct (
   p_manager_id number
) return number is
   v_avg_comm number := 0;
   v_def_comm number := 0;
begin
    -- Compute average commission, treating NULLs as 0
   select nvl(
      avg(nvl(
         commission_pct,
         0
      )),
      0
   )
     into v_avg_comm
     from emp_temp
    where manager_id = p_manager_id;

    -- Determine default commission based on manager ID
   if p_manager_id >= 123 then
      v_def_comm := v_avg_comm * 0.10;
   elsif p_manager_id between 110 and 122 then
      v_def_comm := v_avg_comm * 0.12;
   else
      v_def_comm := 0.1;
   end if;

   return v_def_comm;
exception
   when others then
      dbms_output.put_line('Error calculating default commission: ' || sqlerrm);
      return 0.1; -- fallback value
end;
/

create or replace procedure changeall is
begin
   for emp_rec in (
      select *
        from emp_temp
   ) loop
      if emp_rec.commission_pct is null then
         update emp_temp
            set
            commission_pct = defaultcommissionpct(emp_rec.manager_id)
          where employee_id = emp_rec.employee_id;

         dbms_output.put_line('Updated employee '
                              || emp_rec.employee_id
                              || ': Commission set to '
                              || defaultcommissionpct(emp_rec.manager_id));
      else
         dbms_output.put_line('Employee '
                              || emp_rec.employee_id
                              || ': Commission is valid ('
                              || emp_rec.commission_pct
                              || ')');
      end if;
   end loop;
exception
   when others then
      dbms_output.put_line('An error occurred in changeAll procedure: ' || sqlerrm);
end;
/