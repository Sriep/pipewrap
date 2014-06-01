COPY Added_pSNPs 
(
 base_position, 
 template_base, 
 pSNP_base, 
 frequency,
 start_tandom_repeat
) 
	FROM '/media/sdb2/Projects/Source/Dissertation/c++App/Data/changes.csv'
	WITH DELIMITER AS '	' CSV HEADER;

﻿COPY VarientListing 
(
  read_id,
  read_position,
  read_base,
  template_positon,
  template_base,
  phard,
  ave_auality,
  coverage
) 
	FROM '/media/sdb2/Projects/Source/Dissertation/c++App/Data/varients.csv'
	WITH DELIMITER AS '	' CSV HEADER;
