COPY BioTools (tool_name, tool_tooltip, tool_shell_string,tool_type) 
	FROM '/media/sdb2/Projects/Source/SQL/It1/BioTools.csv'
	WITH CSV HEADER;

COPY ToolOptions (op_command,
	 op_string,
	   op_data_type,	   	   
	   op_short_description,
	   op_long_description,
	   op_optional,
	   op_in_out,
	   op_group,
	   op_postion,
	   op_default_text,
	   op_default_int) 
	FROM '/media/sdb2/Projects/Source/SQL/It1/pIRS simualte.csv'
	WITH CSV HEADER;

COPY ToolOptions (op_command,
	 op_string,
	   op_data_type,	   	   
	   op_short_description,
	   op_long_description,
	   op_optional,
	   op_in_out,
	   op_group,
	   op_postion,
	   op_default_text,
	   op_default_int) 
	FROM '/media/sdb2/Projects/Source/SQL/It1/Stampy.csv'
	WITH CSV HEADER;

COPY ToolOptions (op_command,
	 op_string,
	   op_data_type,	   	   
	   op_short_description,
	   op_long_description,
	   op_optional,
	   op_in_out,
	   op_group,
	   op_postion,
	   op_default_text,
	   op_default_int) 
	FROM '/media/sdb2/Projects/Source/SQL/It1/pSNPGenerator.csv'
	WITH CSV HEADER;

COPY ToolOptions (op_command,
	 op_string,
	   op_data_type,	   	   
	   op_short_description,
	   op_long_description,
	   op_optional,
	   op_in_out,
	   op_group,
	   op_postion,
	   op_default_text,
	   op_default_int) 
	FROM '/media/sdb2/Projects/Source/SQL/It1/samtools.csv'
	WITH CSV HEADER;

COPY ToolOptions (op_command,
	 op_string,
	   op_data_type,	   	   
	   op_short_description,
	   op_long_description,
	   op_optional,
	   op_in_out,
	   op_group,
	   op_postion,
	   op_default_text,
	   op_default_int) 
	FROM '/media/sdb2/Projects/Source/SQL/It1/VarsanSomatic.csv'
	WITH CSV HEADER;
