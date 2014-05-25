DROP TABLE IF EXISTS ToolOptions;
DROP TABLE IF EXISTS BioTools;
CREATE TABLE BioTools (
	id_bio_tools serial  primary key,
	tool_name varchar(45)  unique NOT NULL,
	tool_tooltip varchar(2039) NOT NULL,
	tool_shell_string  varchar(255) unique NOT NULL,
	tool_type smallint  NOT NULL
);

COPY BioTools (tool_name, tool_tooltip, tool_shell_string,tool_type) 
	FROM '/media/sdb2/Projects/Source/SQL/It1/BioTools.csv'
	WITH CSV HEADER;

DROP TABLE IF EXISTS ToolOptions;
CREATE TABLE ToolOptions (
	id_tool_options serial  primary key,
	op_command varchar(63)  NOT NULL REFERENCES BioTools(tool_shell_string),	
	op_string varchar(255),
	op_data_type varchar(31),
	op_short_description varchar(63),
	op_long_description varchar(509),
	op_optional smallint,
	op_in_out smallint,
	op_group smallint,
	op_postion smallint,
	op_default_text varchar(255),
	op_default_int int
);

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
