/*
Write a PL/SQL block that will print ‘Happy Anniversary  X’ for each employee X whose 
hiring date is today. Use cursor FOR loop for the task. 
*/

begin
   for emp_rec in (
      select first_name
             || ' '
             || last_name as full_name
        from employees
       where to_char(
         hire_date,
         'MM-DD'
      ) = to_char(
         sysdate,
         'MM-DD'
      )
   ) loop
      dbms_output.put_line('Happy Anniversary ' || emp_rec.full_name);
   end loop;
end;
/