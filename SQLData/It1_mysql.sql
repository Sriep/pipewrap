CREATE DATABASE `It1` /*!40100 DEFAULT CHARACTER SET latin1 */;

DROP TABLE IF EXISTS ‘BioTools‘;
CREATE TABLE ‘BioTools‘ (
‘id_BioTools‘ int(11) unsigned NOT NULL AUTO_INCREMENT,
‘tool_name‘ char(45)  NOT NULL,
‘short_description‘ char(255) NOT NULL,
‘tool_type tinyint(1) unsigned NOT NULL,
PRIMARY KEY (‘id_BioTools‘)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

INSERT INTO ‘BioTools‘ (‘tool_name‘, ‘short_description‘, ‘tool_type)
VALUES 	("Template", "Template", 0),
		("pIRS", "pIRS", 1),
		("PBSIM", "PBSIM", 1),
		("BWA", "BWA", 0),
		("Stampy", "Stampy", 0),
		("mpileup", "mpileup", 0),
		("varscan", "varscan", 1),
		("genroc", "genroc", 0),
		("sam_bam", "sam_bam", 1);