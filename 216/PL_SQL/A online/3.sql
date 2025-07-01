-- Question 3: Employee Departure Trigger and Leaves Table
-- File: question3_trigger_leaves.sql

-- Step 1: Create the LEAVES table
CREATE TABLE Leaves (
    employee_id NUMBER,
    employee_working_instead_of_him_her NUMBER,
    leave_date DATE DEFAULT SYSDATE,
    CONSTRAINT fk_leaves_emp_id FOREIGN KEY (employee_id) REFERENCES employees(employee_id),
    CONSTRAINT fk_leaves_substitute FOREIGN KEY (employee_working_instead_of_him_her) REFERENCES employees(employee_id)
);

-- Step 2: Create the trigger
CREATE OR REPLACE TRIGGER trg_employee_leaves
    BEFORE DELETE ON employees
    FOR EACH ROW
DECLARE
    v_substitute_emp_id NUMBER := NULL;
    v_is_manager NUMBER := 0;
    v_has_manager NUMBER := 0;
    v_manager_id NUMBER;
    
BEGIN
    -- Check if the employee is a manager (has subordinates)
    SELECT COUNT(*)
    INTO v_is_manager
    FROM employees
    WHERE manager_id = :OLD.employee_id;
    
    -- Check if the employee has a manager
    IF :OLD.manager_id IS NOT NULL THEN
        v_has_manager := 1;
        v_manager_id := :OLD.manager_id;
    END IF;
    
    -- Apply conditions based on priority
    IF v_is_manager > 0 AND v_has_manager = 1 THEN
        -- Condition 3: Both manager and has manager - go for condition 2 (manager logic)
        BEGIN
            SELECT employee_id
            INTO v_substitute_emp_id
            FROM (
                SELECT m.employee_id,
                       ABS(COUNT(e.employee_id) - v_is_manager) as diff
                FROM employees m
                LEFT JOIN employees e ON m.employee_id = e.manager_id
                WHERE m.employee_id != :OLD.employee_id
                AND EXISTS (SELECT 1 FROM employees WHERE manager_id = m.employee_id)
                GROUP BY m.employee_id
                ORDER BY diff ASC
            )
            WHERE ROWNUM = 1;
        EXCEPTION
            WHEN NO_DATA_FOUND THEN
                v_substitute_emp_id := NULL;
        END;
        
    ELSIF v_is_manager > 0 THEN
        -- Condition 2: Employee is a manager
        BEGIN
            SELECT employee_id
            INTO v_substitute_emp_id
            FROM (
                SELECT m.employee_id,
                       ABS(COUNT(e.employee_id) - v_is_manager) as diff
                FROM employees m
                LEFT JOIN employees e ON m.employee_id = e.manager_id
                WHERE m.employee_id != :OLD.employee_id
                AND EXISTS (SELECT 1 FROM employees WHERE manager_id = m.employee_id)
                GROUP BY m.employee_id
                ORDER BY diff ASC
            )
            WHERE ROWNUM = 1;
        EXCEPTION
            WHEN NO_DATA_FOUND THEN
                v_substitute_emp_id := NULL;
        END;
        
    ELSIF v_has_manager = 1 THEN
        -- Condition 1: Employee has a manager
        BEGIN
            SELECT employee_id
            INTO v_substitute_emp_id
            FROM (
                SELECT employee_id,
                       ABS(salary - :OLD.salary) as salary_diff
                FROM employees
                WHERE manager_id = v_manager_id
                AND employee_id != :OLD.employee_id
                ORDER BY salary_diff ASC
            )
            WHERE ROWNUM = 1;
        EXCEPTION
            WHEN NO_DATA_FOUND THEN
                v_substitute_emp_id := NULL;
        END;
    END IF;
    
    -- Insert record into Leaves table
    INSERT INTO Leaves (
        employee_id,
        employee_working_instead_of_him_her,
        leave_date
    ) VALUES (
        :OLD.employee_id,
        v_substitute_emp_id,
        SYSDATE
    );
    
EXCEPTION
    WHEN OTHERS THEN
        -- Log error and insert with NULL substitute
        INSERT INTO Leaves (
            employee_id,
            employee_working_instead_of_him_her,
            leave_date
        ) VALUES (
            :OLD.employee_id,
            NULL,
            SYSDATE
        );
END;
/

-- Step 3: Create a view to easily see the leaves data
CREATE OR REPLACE VIEW v_employee_leaves AS
SELECT 
    l.employee_id as departed_employee_id,
    CASE 
        WHEN l.employee_working_instead_of_him_her IS NOT NULL THEN
            l.employee_working_instead_of_him_her
        ELSE NULL
    END as substitute_employee_id,
    CASE 
        WHEN l.employee_working_instead_of_him_her IS NOT NULL THEN
            (SELECT first_name || ' ' || last_name 
             FROM employees 
             WHERE employee_id = l.employee_working_instead_of_him_her)
        ELSE 'No Substitute Found'
    END as substitute_employee_name,
    l.leave_date
FROM Leaves l
ORDER BY l.leave_date DESC;

-- Test queries (DO NOT RUN THE DELETE unless you want to test!)
-- WARNING: The DELETE will actually remove an employee from the database!

-- To test the trigger safely, first check what employees exist:
-- SELECT employee_id, first_name, last_name, manager_id FROM employees WHERE employee_id BETWEEN 200 AND 210;

-- To test (CAREFUL - this will delete an employee):
-- DELETE FROM employees WHERE employee_id = 206;

-- To see the results:
-- SELECT * FROM Leaves;
-- SELECT * FROM v_employee_leaves;

-- To check if the table was created successfully:
SELECT 'Leaves table created successfully' as status FROM dual
WHERE EXISTS (SELECT 1 FROM user_tables WHERE table_name = 'LEAVES');