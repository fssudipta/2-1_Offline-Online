create or replace procedure rank_jobs is
    -- Define a cursor to get job statistics with ranking
   cursor cur_job_rank is
   select job_id,
          job_title,
          count(*) as employee_count,
          round(
             avg(salary),
             2
          ) as avg_salary,
          max(salary) as max_salary,
          min(salary) as min_salary,
          rank()
          over(
              order by count(*) desc,
                       avg(salary) desc
          ) as job_rank
     from employees
     join jobs
   using ( job_id )
    group by job_id,
             job_title
    order by job_rank;

begin
   for job_rec in cur_job_rank loop
      dbms_output.put_line(rpad(
         job_rec.job_rank,
         6
      )
                           || rpad(
         job_rec.job_title,
         35
      )
                           || rpad(
         job_rec.employee_count,
         6
      )
                           || rpad(
         job_rec.avg_salary,
         10
      )
                           || rpad(
         job_rec.max_salary,
         10
      )
                           || rpad(
         job_rec.min_salary,
         10
      ));
   end loop;
end;
/