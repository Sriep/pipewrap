COPY BioTools (tool_name, tool_tooltip, tool_shell_string, path_to_tool, tool_type) 
	FROM '/media/sdb2/Projects/Source/Dissertation/SQLData/BioTools.csv'
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
	FROM '/media/sdb2/Projects/Source/Dissertation/SQLData/pIRS simualte.csv'
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
	FROM '/media/sdb2/Projects/Source/Dissertation/SQLData/Stampy.csv'
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
	FROM '/media/sdb2/Projects/Source/Dissertation/SQLData/pSNPGenerator.csv'
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
	FROM '/media/sdb2/Projects/Source/Dissertation/SQLData/samtools.csv'
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
	FROM '/media/sdb2/Projects/Source/Dissertation/SQLData/VarsanSomatic.csv'
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
	FROM '/media/sdb2/Projects/Source/Dissertation/SQLData/pbsim.csv'
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
	FROM '/media/sdb2/Projects/Source/Dissertation/SQLData/alchemy.csv'
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
	FROM '/media/sdb2/Projects/Source/Dissertation/SQLData/Plotroc.csv'
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
	FROM '/media/sdb2/Projects/Source/Dissertation/SQLData/VarientLister.csv'
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
	FROM '/media/sdb2/Projects/Source/Dissertation/SQLData/blasr.csv'
	WITH CSV HEADER;
