---------1-----------
   SET SERVEROUTPUT ON;

declare
   v_count number := 0;
begin
   for emp_rec in (
      select employee_id,
             last_name,
             department_id,
             salary,
             hire_date
        from employees
       where manager_id is not null
   ) loop
      if ( months_between(
         sysdate,
         emp_rec.hire_date
      ) / 12 ) >= 24 then
         update employees
            set
            salary = salary * 1.15
          where employee_id = emp_rec.employee_id;

         dbms_output.put_line('Updated: '
                              || emp_rec.last_name
                              || ', Dept: '
                              || emp_rec.department_id
                              || ', Old Salary: '
                              || emp_rec.salary
                              || ', New Salary: '
                              || emp_rec.salary * 1.15);
         v_count := v_count + 1;
      end if;
   end loop;

   dbms_output.put_line('Total Managers Updated: ' || v_count);
end;
/

-----------2------------
create or replace procedure interchange_salary (
   eid1 in number,
   eid2 in number
) is
   sal1 number;
   sal2 number;
begin
    -- Get current salaries
   select salary
     into sal1
     from employees
    where employee_id = eid1;
   select salary
     into sal2
     from employees
    where employee_id = eid2;

    -- Swap salaries
   update employees
      set
      salary = sal2
    where employee_id = eid1;
   update employees
      set
      salary = sal1
    where employee_id = eid2;

   dbms_output.put_line('Employee '
                        || eid1
                        || ': Salary changed from '
                        || sal1
                        || ' to '
                        || sal2);
   dbms_output.put_line('Employee '
                        || eid2
                        || ': Salary changed from '
                        || sal2
                        || ' to '
                        || sal1);

exception
   when no_data_found then
      dbms_output.put_line('One or both employee IDs are invalid.');
   when others then
      dbms_output.put_line('An error occurred: ' || sqlerrm);
end;
/
declare begin
   interchange_salary(
      100,
      101
   );
end;
/

-----------3------------
CREATE OR REPLACE FUNCTION LONG_TERM_ACCOUNTANTS
RETURN NUMBER
IS
    CURSOR cur_employees IS
        SELECT e.LAST_NAME, e.HIRE_DATE
        FROM EMPLOYEES e
        JOIN DEPARTMENTS d ON e.DEPARTMENT_ID = d.DEPARTMENT_ID
        WHERE d.DEPARTMENT_NAME = 'Accounting';

    v_count NUMBER := 0;
BEGIN
    FOR emp_rec IN cur_employees LOOP
        IF (MONTHS_BETWEEN(SYSDATE, emp_rec.HIRE_DATE) / 12) > 20 THEN
            DBMS_OUTPUT.PUT_LINE(emp_rec.LAST_NAME || ' has worked more than 20 years.');
            v_count := v_count + 1;
        END IF;
    END LOOP;

    RETURN v_count;
END;
/
DECLARE
    v_result NUMBER;
BEGIN
    v_result := LONG_TERM_ACCOUNTANTS;
    DBMS_OUTPUT.PUT_LINE('Number of long-term accountants: ' || v_result);
END;
/
--------------4----------------
-- Step 1: Copy Table
CREATE TABLE EMPLOYEE_2 AS SELECT * FROM EMPLOYEES;

-- Step 2: Create Trigger
CREATE OR REPLACE TRIGGER NO_DEC
BEFORE UPDATE OF SALARY ON EMPLOYEE_2
FOR EACH ROW
BEGIN
    IF :NEW.SALARY < :OLD.SALARY THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error: Salary cannot be decreased.');
    END IF;
END;
/
