-- Question 2: RANK_JOBS Procedure
-- File: question2_rank_jobs.sql

CREATE OR REPLACE PROCEDURE RANK_JOBS IS
    CURSOR job_cursor IS
        SELECT 
            j.job_title,
            COUNT(e.employee_id) as total_employees,
            ROUND(AVG(e.salary), 2) as avg_salary,
            MAX(e.salary) as max_salary,
            MIN(e.salary) as min_salary
        FROM jobs j
        LEFT JOIN employees e ON j.job_id = e.job_id
        GROUP BY j.job_id, j.job_title
        ORDER BY COUNT(e.employee_id) DESC, AVG(e.salary) DESC;
    
    v_rank NUMBER := 1;
    v_prev_emp_count NUMBER := -1;
    v_prev_avg_salary NUMBER := -1;
    v_actual_rank NUMBER := 1;
    
BEGIN
    -- Enable DBMS_OUTPUT
    DBMS_OUTPUT.ENABLE(1000000);
    
    DBMS_OUTPUT.PUT_LINE('=== JOB RANKINGS ===');
    DBMS_OUTPUT.PUT_LINE(RPAD('Rank', 6) || RPAD('Job Title', 35) || 
                        RPAD('Total Employees', 17) || RPAD('Average Salary', 16) || 
                        RPAD('Max Salary', 12) || 'Min Salary');
    DBMS_OUTPUT.PUT_LINE(RPAD('-', 6, '-') || RPAD('-', 35, '-') || 
                        RPAD('-', 17, '-') || RPAD('-', 16, '-') || 
                        RPAD('-', 12, '-') || RPAD('-', 12, '-'));
    
    FOR job_rec IN job_cursor LOOP
        -- Handle ranking logic
        IF job_rec.total_employees != v_prev_emp_count OR 
           NVL(job_rec.avg_salary, 0) != NVL(v_prev_avg_salary, 0) THEN
            v_rank := v_actual_rank;
        END IF;
        
        -- Display job information
        DBMS_OUTPUT.PUT_LINE(
            RPAD(v_rank, 6) || 
            RPAD(job_rec.job_title, 35) || 
            RPAD(job_rec.total_employees, 17) || 
            RPAD(NVL(TO_CHAR(job_rec.avg_salary), 'N/A'), 16) || 
            RPAD(NVL(TO_CHAR(job_rec.max_salary), 'N/A'), 12) || 
            NVL(TO_CHAR(job_rec.min_salary), 'N/A')
        );
        
        -- Update previous values for next iteration
        v_prev_emp_count := job_rec.total_employees;
        v_prev_avg_salary := NVL(job_rec.avg_salary, 0);
        v_actual_rank := v_actual_rank + 1;
    END LOOP;
    
    DBMS_OUTPUT.PUT_LINE('');
    DBMS_OUTPUT.PUT_LINE('Ranking completed successfully!');
    
EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('ERROR: An error occurred while ranking jobs: ' || SQLERRM);
END;
/

-- Test the procedure
-- Uncomment the line below to test:
 EXEC RANK_JOBS;