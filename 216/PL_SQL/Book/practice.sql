begin
   dbms_output.put_line('hello sensei');
end;
/

declare
   ename varchar2(100);
begin
   select ( first_name
            || ' '
            || last_name )
     into ename
     from employees
    where employee_id = 100;
   dbms_output.put_line('the name is: ' || ename);
end;
/

declare
   jdate   date;
   months  number;
   rmonths number;
begin
   select hire_date
     into jdate
     from employees
    where employee_id = 100;
   months := months_between(
      sysdate,
      jdate
   );
   rmonths := round(
      months,
      0
   );
   dbms_output.put_line('the employee worked '
                        || rmonths
                        || ' months.');
end;
/

show ERRORS;

declare
   jdate date;
   years number;
begin
   select hire_date
     into jdate
     from employees
    where employee_id = 100;
   years := ( months_between(
      sysdate,
      jdate
   ) / 12 );
   if years >= 10 then
      dbms_output.put_line('the employee worked here for 10 years or more.');
   else
      dbms_output.put_line('ehhe.');
   end if;
end;
/

declare
   esalary number;
begin
   select salary
     into esalary
     from employees
    where employee_id = 100;
   if esalary < 1000 then
      dbms_output.put_line('yada yada');
   elsif esalary >= 1000 then
      dbms_output.put_line('whaaa');
   else
      dbms_output.put_line('daya, pata lagao');
   end if;
exception
   when no_data_found then
      dbms_output.put_line('ehheereeee.');
   when others then
      dbms_output.put_line('aro boro ehhe.');
end;
/

declare begin
   for i in 1..10 loop
      dbms_output.put_line(i);
   end loop;
end;
/

declare
   i number;
begin
   i := 1;
   while i <= 10 loop
      dbms_output.put_line('There are '
                           || i
                           || ' items in stock.');
      i := i + 1;
   end loop;
end;
/

declare
   years   number;
   counter number;
begin
   counter := 0;
   for r in (
      select hire_date
        from employees
   ) loop
      years := ( months_between(
         sysdate,
         r.hire_date
      ) / 12 );
      if years >= 10 then
         counter := counter + 1;
      end if;
   end loop;
   dbms_output.put_line('number: ' || counter);
end;
/

declare
   years   number;
   counter number;
   old_sal number;
   new_sal number;
begin
   counter := 0;
   for r in (
      select employee_id,
             salary,
             hire_date
        from employees
   ) loop
      old_sal := r.salary;
      years := ( months_between(
         sysdate,
         hire_date
      ) / 12 );
      if years >= 10 then
         update employees
            set
            salary = salary * 1.15
          where employee_id = r.employee_id;
      end if;
      select salary
        into new_sal
        from employees
       where employee_id = r.employee_id;
      dbms_output.put_line('Employee id: '
                           || r.employee_id
                           || ' salary: '
                           || old_sal
                           || ' -> '
                           || new_sal);
   end loop;
end;
/

create or replace procedure is_senior_employee (
   eid in varchar2
) is
   jdate date;
   years number;
begin
   select hire_date
     into jdate
     from employees
    where employee_id = eid;
   years := ( months_between(
      sysdate,
      jdate
   ) / 12 );
   if years >= 10 then
      dbms_output.put_line('the employee worked 10 years or more');
   else
      dbms_output.put_line('the employee worked less than 10 years');
   end if;
exception
   when no_data_found then
      dbms_output.put_line('no employee found.');
   when too_many_rows then
      dbms_output.put_line('etogula keno');
   when others then
      dbms_output.put_line('kuch to garbar hain.');
end;
/
--exec is_senior_employee;

declare begin
   is_senior_employee(50);
end;
/
SHOW ERRORS;

create or replace procedure is_someone (
   eid in varchar2,
   msg out varchar2
) is
   jdate date;
   years number;
begin
   select hire_date
     into jdate
     from employees
    where employee_id = eid;
   years := ( months_between(
      sysdate,
      jdate
   ) / 12 );
   if years >= 10 then
      msg := 'the employee worked 10 years or more';
   else
      msg := 'the employee worked less than 10 years';
   end if;
exception
   when no_data_found then
      dbms_output.put_line('no employee found.');
   when too_many_rows then
      dbms_output.put_line('etogula keno');
   when others then
      dbms_output.put_line('kuch to garbar hain.');
end;
/

declare
   message varchar2;
begin
   is_someone(
      10000,
      message
   );
   dbms_output.put_line(message);
end;
/

create or replace function is_somebody (
   eid in varchar2
) return varchar2 is
   jdate date;
   years number;
   msg   varchar2(100);
begin
   select hire_date
     into jdate
     from employees
    where employee_id = eid;
   years := ( months_between(
      sysdate,
      jdate
   ) / 12 );
   if years >= 10 then
      msg := 'the employee worked 10 years or more';
   else
      msg := 'the employee worked less than 10 years';
   end if;
   return msg;
exception
   when no_data_found then
      dbms_output.put_line('no employee found.');
   when too_many_rows then
      dbms_output.put_line('etogula keno');
   when others then
      dbms_output.put_line('kuch to garbar hain.');
end;
/

declare
   message varchar2(100);
begin
   message := is_somebody(10000);
   dbms_output.put_line(message);
end;
/

create or replace function get_senior_employee (
   eid in varchar2
) return varchar2 is
   ecount number;
   jdate  date;
   years  number;
   msg    varchar2(100);
begin --Inner PL/SQL block
   begin
      select count(*)
        into ecount
        from employees
       where employee_id = eid;
   end;
   if ecount = 0 then
      msg := 'No employee found.';
   elsif ecount > 1 then
      msg := 'More than one employee found.';
   else
      select hire_date
        into jdate
        from employees
       where employee_id = eid;
      years := ( months_between(
         sysdate,
         jdate
      ) / 12 );
      if years >= 10 then
         msg := 'The employee worked 10 years or more';
      else
         msg := 'The employee worked less than 10 years';
      end if;
   end if;
   return msg;
end;
/

-----------------trigger----------------------
create table students (
   student_name varchar2(250),
   cgpa         number
);

create or replace trigger hello_world after
   insert on students
declare begin
   dbms_output.put_line('Hello World');
end;
/

insert into students values ( 'Fahim Hasan',
                              3.71 );
insert into students values ( 'Ahmed Nahiyan',
                              3.80 ); 

--This trigger will run before insert on STUDENTS table 
create or replace trigger hello_world2 before
   insert on students
declare begin
   dbms_output.put_line('Hello World2');
end;
/ 
 --This trigger will run after an insert or a delete statement on STUDENTS table 

create or replace trigger hello_world3 after
   insert or delete on students
declare begin
   dbms_output.put_line('Hello World3');
end;
/ 
 --The following trigger will run after an update statement on STUDENTS table. 
 --Added with that, this trigger will only run when update is performed on the 
 --CGPA column. 

create or replace trigger hello_world4 after
   update of cgpa on students
declare begin
   dbms_output.put_line('Hello World4');
end;
/ 
 --The following trigger will run after an update operation on the STUDENTS table. 
 --Added with that, the trigger will run once for each row. The previous rows will 
 --run once for the whole statement, where this trigger will run N times if N rows 
 --are affected by the SQL statement. 

create or replace trigger hello_world5 after
   update of cgpa on students
   for each row
declare begin
   dbms_output.put_line('Hello World5');
end;
/

insert into students values ( 'Shakib Ahmed',
                              3.63 ); 
--This will run HELLO_WORLD, HELLO_WORLD2, HELLO_WORLD3 

delete from students
 where cgpa < 3.65; --This will run HELLO_WOLRD3 

update students
   set
   cgpa = cgpa + 0.01
 where student_name like '%Shakib%';  
--This will run HELLO_WORLD4, but will not run HELLO_WORLD5!!! Why? Because 
--HELLO_WORLD5 is declared with FOR EACH ROW clause. This means trigger should be 
--run for each row affected. Since, the above statement does not update any row 
--(as the previous DELETE operation already deleted that row from the table)  
--it will not run HELLO_WORLD5! 

update students
   set
   student_name = 'Fahim Ahmed'
 where student_name = 'Fahim Hasan'; 
--This will not run any trigger. Although HELLO_TRIGGER4 is declared to be run 
--after an update operation, the trigger will not run because the update is done 
--on the column STUDENT_NAME rather than CGPA 

update students
   set
   cgpa = cgpa + 0.01; 
--This will run both HELLO_WORLD4 and HELLO_WORLD5 trigger. However, HELLO_WORLD5 
--will run twice! Why? Because two rows will be affected by the SQL statement! 

-------------new old bujhinai----------------
create or replace trigger old_new_test before
   insert or update or delete on students
   for each row
declare begin
   dbms_output.put_line(':OLD.CGPA = ' || :old.cgpa);
   dbms_output.put_line(':NEW.CGPA = ' || :new.cgpa);
end;
/
 --Issue the following SQL statements and view the dbms outputs 
insert into students values ( 'SOUMIK SARKAR',
                              3.85 );
update students
   set
   cgpa = cgpa + 0.02;
delete from students
 where cgpa < 3.90;