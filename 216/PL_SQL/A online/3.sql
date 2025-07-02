create table leaves (
   employee_id                            number primary key,
   employee_working_instead_of_him_or_her number,
   leave_date                             date
);

create or replace trigger handle_employee_delete after
   delete on employees
   for each row
declare
   v_substitute_id number;
begin
    -- Condition 3: Prefer manager condition if both apply
   if :old.manager_id is not null then
        -- Try to find someone under same manager with closest salary
      select employee_id
        into v_substitute_id
        from (
         select employee_id
           from employees
          where employee_id != :old.employee_id
            and manager_id = :old.manager_id
            and salary is not null
          order by abs(salary - :old.salary)
          fetch first 1 row only
      );

   elsif exists (
      select 1
        from employees
       where manager_id = :old.employee_id
   ) then
        -- Try to find another manager with closest subordinate count
      select e.employee_id
        into v_substitute_id
        from employees e
       where exists (
         select 1
           from employees sub
          where sub.manager_id = e.employee_id
      )
         and e.employee_id != :old.employee_id
       order by abs((
         select count(*)
           from employees
          where manager_id = e.employee_id
      ) -(
         select count(*)
           from employees
          where manager_id = :old.employee_id
      ))
       fetch first 1 row only;

   else
      v_substitute_id := null;
   end if;

    -- Insert into LEAVES table
   insert into leaves (
      employee_id,
      employee_working_instead_of_him_or_her,
      leave_date
   ) values ( :old.employee_id,
              v_substitute_id,
              sysdate );

exception
   when no_data_found then
      insert into leaves (
         employee_id,
         employee_working_instead_of_him_or_her,
         leave_date
      ) values ( :old.employee_id,
                 null,
                 sysdate );
   when others then
      dbms_output.put_line('Error handling deletion of employee '
                           || :old.employee_id
                           || ': '
                           || sqlerrm);
end;
/