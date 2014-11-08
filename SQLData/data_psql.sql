COPY BioTools (tool_name, tool_tooltip, tool_shell_string, path_to_tool, tool_type) 
	FROM '/home/piers/Documents/pipewrap/SQLData/BioTools.csv'
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
	FROM '/home/piers/Documents/pipewrap/SQLData/pIRS simualte.csv'
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
	FROM '/home/piers/Documents/pipewrap/SQLData/Stampy.csv'
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
	FROM '/home/piers/Documents/pipewrap/SQLData/pSNPGenerator.csv'
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
	FROM '/home/piers/Documents/pipewrap/SQLData/samtools.csv'
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
	FROM '/home/piers/Documents/pipewrap/SQLData/VarsanSomatic.csv'
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
	FROM '/home/piers/Documents/pipewrap/SQLData/pbsim.csv'
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
	FROM '/home/piers/Documents/pipewrap/SQLData/alchemy.csv'
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
	FROM '/home/piers/Documents/pipewrap/SQLData/Plotroc.csv'
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
	FROM '/home/piers/Documents/pipewrap/SQLData/VarientLister.csv'
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
	FROM '/home/piers/Documents/pipewrap/SQLData/blasr.csv'
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
	FROM '/home/piers/Documents/pipewrap/SQLData/bwa.csv'
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
	FROM '/home/piers/Documents/pipewrap/SQLData/simbas.csv'
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
	FROM '/home/piers/Documents/pipewrap/SQLData/AnalyseBax.csv'
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
	FROM '/home/piers/Documents/pipewrap/SQLData/lofreq.csv'
	WITH CSV HEADER;
