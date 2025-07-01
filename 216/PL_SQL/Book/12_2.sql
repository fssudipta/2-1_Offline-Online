/*Write an example PL/SQL block that inserts a new arbitrary row to the COUNTRIES table. 
The block should handle the exception DUP_VAL_ON_INDEX and OTHERS. Run the block 
for different COUNTRY_ID and observe the cases when above exception occurs. */

declare
   v_country_id   char(2) := 'XX';
   v_country_name varchar2(40) := 'Test Country';
   v_region_id    number := 1;
begin
   insert into countries (
      country_id,
      country_name,
      region_id
   ) values ( v_country_id,
              v_country_name,
              v_region_id );
   dbms_output.put_line('Insert successful: Country ID '
                        || v_country_id
                        || ' added.');
exception
   when dup_val_on_index then
      dbms_output.put_line('Error: Duplicate value on index. Country ID '
                           || v_country_id
                           || ' already exists.');
   when others then
      dbms_output.put_line('An unexpected error occurred: ' || sqlerrm);
end;
/