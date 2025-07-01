------problem 1------
--Write a PL/SQL trigger LOG_CGPA_UPDATE. This trigger will log all updates done on the CGPA 
--column of STUDENTS table. The trigger will save current user’s name, current system date and time in a 
--log table named LOG_TABLE_CGPA_UPDATE.

create table log_table_cgpa_update (
   username varchar2(25),
   datetime date
);
create or replace trigger log_cgpa_update after
   update of cgpa on students
declare
   username varchar2(25);
begin
   username := user; --USER is a function that returns current username 
   insert into log_table_cgpa_update values ( username,
                                              sysdate );
end;
/ 
--First update 
update students
   set
   cgpa = cgpa + 0.01; --Another update 
update students
   set
   cgpa = cgpa - 0.01; --View the rows inserted by the trigger 
select *
  from log_table_cgpa_update;

------problem 2------
/*Write a PL/SQL trigger BACKUP_DELETED_STUDENTS. This trigger will save all records that are 
deleted from the STUDENTS table into a backup table named STUDENTS_DELETED. The trigger will 
save student’s record along with current user’s name and current system date and time. */

create table students_deleted (
   student_name varchar2(25),
   cgpa         number,
   username     varchar2(25),
   datetime     date
);

create or replace trigger backup_deleted_students before
   delete on students
   for each row
declare
   v_name     varchar2(25);
   v_username varchar2(25);
   v_cgpa     number;
   v_datetime date;
begin
   v_name := :old.student_name;
   v_cgpa := :old.cgpa;
   v_username := user;
   v_datetime := sysdate;
   insert into students_deleted values ( v_name,
                                         v_cgpa,
                                         v_username,
                                         v_datetime );
end;
/

-----------problem 3---------------
/*Write a PL/SQL trigger CORRECT_STUDENT_NAME. This trigger will be used to correct the text case 
of the student names when it is going to be inserted. So, this will be a BEFORE INSERT trigger. The trigger 
will change the case of the student’s name to INITCAP format if it was not given by the user in the INSERT 
statement. The trigger will thus ensure that all names stored in the STUDENTS table will be in a consistent 
format. */

create or replace trigger correct_student_name before
   insert on students
   for each row
declare begin
   :new.student_name := initcap(:new.student_name);
end;
/ --Issue the following SQL statements and then view the rows of STUDENTS table 
insert into students values ( 'SHAkil ahMED',
                              3.80 );
insert into students values ( 'masum billah',
                              3.60 );