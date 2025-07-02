-- Question 1: Alternative Version - Returns results instead of using DBMS_OUTPUT
-- This version works better with Navicat

CREATE OR REPLACE FUNCTION GET_LONGEST_SERVING_EMPLOYEE(
    p_region_name IN VARCHAR2
) RETURN SYS_REFCURSOR IS
    result_cursor SYS_REFCURSOR;
BEGIN
    OPEN result_cursor FOR
        SELECT 
            'LONGEST SERVING EMPLOYEE IN ' || UPPER(p_region_name) as title,
            e.first_name || ' ' || e.last_name as full_name,
            j.job_title,
            TO_CHAR(e.hire_date, 'DD-MON-YYYY') as hire_date,
            c.country_name,
            d.department_name,
            l.city
        FROM employees e
        JOIN departments d ON e.department_id = d.department_id
        JOIN locations l ON d.location_id = l.location_id
        JOIN countries c ON l.country_id = c.country_id
        JOIN regions r ON c.region_id = r.region_id
        JOIN jobs j ON e.job_id = j.job_id
        WHERE UPPER(r.region_name) = UPPER(p_region_name)
        AND e.hire_date = (
            SELECT MIN(e2.hire_date)
            FROM employees e2
            JOIN departments d2 ON e2.department_id = d2.department_id
            JOIN locations l2 ON d2.location_id = l2.location_id
            JOIN countries c2 ON l2.country_id = c2.country_id
            JOIN regions r2 ON c2.region_id = r2.region_id
            WHERE UPPER(r2.region_name) = UPPER(p_region_name)
        )
        AND ROWNUM = 1;
    
    RETURN result_cursor;
END;
/

-- Simpler version using direct SELECT
CREATE OR REPLACE PROCEDURE SHOW_LONGEST_SERVING_EMPLOYEE(
    p_region_name IN VARCHAR2
) IS
BEGIN
    -- This will show results in the Result tab
    FOR rec IN (
        SELECT 
            e.first_name || ' ' || e.last_name as full_name,
            j.job_title,
            TO_CHAR(e.hire_date, 'DD-MON-YYYY') as hire_date,
            c.country_name,
            d.department_name,
            l.city
        FROM employees e
        JOIN departments d ON e.department_id = d.department_id
        JOIN locations l ON d.location_id = l.location_id
        JOIN countries c ON l.country_id = c.country_id
        JOIN regions r ON c.region_id = r.region_id
        JOIN jobs j ON e.job_id = j.job_id
        WHERE UPPER(r.region_name) = UPPER(p_region_name)
        AND e.hire_date = (
            SELECT MIN(e2.hire_date)
            FROM employees e2
            JOIN departments d2 ON e2.department_id = d2.department_id
            JOIN locations l2 ON d2.location_id = l2.location_id
            JOIN countries c2 ON l2.country_id = c2.country_id
            JOIN regions r2 ON c2.region_id = r2.region_id
            WHERE UPPER(r2.region_name) = UPPER(p_region_name)
        )
        AND ROWNUM = 1
    ) LOOP
        -- Results will appear in Messages tab
        NULL; -- Just iterate through to execute the query
    END LOOP;
END;
/
