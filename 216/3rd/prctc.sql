select last_name,
       round(
          (sysdate - hire_date) / 365,
          4
       ) weeks_employed
  from employees
 where department_id = 80;

select round(
   sysdate + 1,
   'month'
)
  from dual;

select *
  from employees
 where nvl(
   commission_pct,
   -1
) = - 1;

select to_char(
   sysdate,
   'MONTH ddspth, year'
)
  from dual;

select street_address,
       postal_code
  from locations
 order by postal_code asc;

select last_name,
       to_char(
          hire_date,
          'dd-mon-yyyy'
       ) hd
  from employees
 where hire_date < to_date('01-jan-2020','dd-mon-yyyy')
 order by hire_date asc;

select job_id,
       max(salary),
       min(salary),
       avg(salary)
  from employees
 group by job_id;

select job_id,
       max(distinct salary),
       min(distinct salary),
       sum(distinct salary),
       count(distinct department_id)
  from employees
 where department_id = 80
 group by job_id
 order by job_id asc;

select e1.last_name employee,
       e2.last_name manager
  from employees e1
  join employees e2
on ( e1.manager_id = e2.employee_id );

select last_name,
       salary
  from employees
 where salary = (
   select max(salary)
     from employees
);

select e1.job_id,
       count(*)
  from employees e1
 where 3 <= (
   select count(*)
     from employees e2
    where e2.salary < e1.salary
)
 group by e1.job_id;

select e.last_name,
       e.salary,
       d.minsal,
       d.maxsal
  from employees e,
       (
          select department_id as dept,
                 min(salary) minsal,
                 max(salary) maxsal
            from employees
           group by department_id
       ) d
 where ( e.department_id = d.dept )
 order by e.salary;

 SELECT employee_id, job_id from EMPLOYEES
 UNION ALL
 (SELECT EMPLOYEE_ID, JOB_ID from JOB_HISTORY);

 SELECT EMPLOYEE_ID, JOB_ID 
FROM EMPLOYEES 
INTERSECT 
( 
SELECT EMPLOYEE_ID, JOB_ID 
FROM JOB_HISTORY 
) ; 

SELECT JOB_ID, DECODE(JOB_TITLE,'Marketing Manager','Manager','Not a manager') 
FROM JOBS; 

SELECT JOB_ID, DECODE(INSTR(UPPER(JOB_TITLE),'MANAGER'),0,'Not a manager', 
'Manager') 
FROM JOBS;

SELECT DEPARTMENT_ID,  
SUM(CASE WHEN SALARY > 15000 THEN 1 ELSE 0 END) "A+", 
SUM(CASE WHEN SALARY >= 10000 AND SALARY < 15000 THEN 1 ELSE 0 END) "A", 
SUM(CASE WHEN SALARY >= 5000 AND SALARY < 10000 THEN 1 ELSE 0 END) "B", 
SUM(CASE WHEN SALARY < 5000 THEN 1 ELSE 0 END) "C" 
FROM EMPLOYEES 
GROUP BY DEPARTMENT_ID 
ORDER BY DEPARTMENT_ID ASC;

SELECT 
    'Q' || TO_CHAR(hire_date, 'Q') AS quarter,
    COUNT(*) AS num_employees
FROM 
    employees
GROUP BY 
    TO_CHAR(hire_date, 'Q')
ORDER BY 
    quarter;
