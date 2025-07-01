/*
In Oracle, there is a function TO_NUMBER that converts a VARCHAR2 value to a numeric 
value.  If the input to this function is not a valid number, then this function throws an exception. 
This is a problem in a SQL query because the whole query would not produce any result if one 
row generates an exception. So, your job is to write a PL/SQL function ISNUMBER that 
receives an input VARCHAR2 value and checks whether the input can be converted to a valid 
number. If the input can be converted to a valid number than ISNUMBER should return ‘YES’, 
otherwise ISNUMBER should return ‘NO’. 
*/

create or replace function isnumber (
   p_input in varchar2
) return varchar2 is
   v_number number;
begin
   v_number := to_number ( p_input );
   return 'YES';
exception
   when value_error then
      return 'NO';
   when others then
      return 'NO';
end;
/

declare
   v_result varchar2(10);
begin
   v_result := isnumber('yes');
   dbms_output.put_line('Result: ' || v_result);
end;
/

/*
Write a trigger HELLO_WORLD6 that will run after a deletion operation on the STUDENTS 
table. The trigger should be a ROW LEVEL trigger. 
*/
create or replace trigger hello_world6 after
   delete on students
   for each row
begin
   dbms_output.put_line('Hello World! A student was deleted.');
end;
/

/*
Write down a PL/SQL trigger on STUDENTS table. The trigger will ensure that whenever a 
new row is inserted in the STUDENTS table, the name of the student contains only alphabetic 
characters. Name your trigger INVALID_NAME. If the name is valid, then insertion should 
be allowed. However, if the name is invalid, then insertion should be denied. To deny insertion, 
you can throw an exception from the trigger that would halt the insertion operation. 
*/

/*
Write a trigger that will save a student records in a table named LOW_CGPA_STUDENTS 
which contain only one column to store student’s names. The trigger will work before an update 
operation or an insert operation. Whenever the update operation results in a CGPA value less 
than 2.0, the trigger will be fired and the trigger will save the students name in the 
LOW_CGPA_STUDENTS table. Similarly, when an insert operation inserts a new row with 
CGPA less than 2.0, the corresponding row must be saved in the LOW_CGPA_STUDENTS 
table. 
*/
create table low_cgpa_students (
   student_name varchar2(100)
);

create or replace trigger trg_low_cgpa_students before
   insert or update on students
   for each row
begin
   if :new.cgpa < 2.0 then
      insert into low_cgpa_students ( student_name ) values ( :new.student_name );
   end if;
end;
/
insert into students (
   student_name,
   cgpa
) values ( 'Ariana',
           3.85 );
insert into students (
   student_name,
   cgpa
) values ( 'Hasan',
           2.45 );
insert into students (
   student_name,
   cgpa
) values ( 'Munni',
           1.90 );  -- Low CGPA
insert into students (
   student_name,
   cgpa
) values ( 'Borsha',
           3.10 );
insert into students (
   student_name,
   cgpa
) values ( 'Ratan',
           1.70 );  -- Low CGPA

insert into students (
   student_name,
   cgpa
) values ( 'Nirob Mia',
           1.95 );

update students
   set
   cgpa = 1.80
 where student_name = 'Rafiul';