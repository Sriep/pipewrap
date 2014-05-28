DROP TABLE IF EXISTS ToolOptions;
DROP TABLE IF EXISTS BioTools;
CREATE TABLE BioTools (
	id_bio_tools serial  primary key,
	tool_name varchar(45)  unique NOT NULL,
	tool_tooltip varchar(2039) NOT NULL,
	tool_shell_string  varchar(255) unique NOT NULL,
	path_to_tool varchar(255),
	tool_type smallint  NOT NULL
);

DROP TABLE IF EXISTS ToolOptions;
CREATE TABLE ToolOptions (
	id_tool_options serial  primary key,
	op_command varchar(63)  REFERENCES BioTools(tool_shell_string),	
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
