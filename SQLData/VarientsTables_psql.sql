DROP TABLE IF EXISTS Added_pSNPs;
CREATE TABLE Added_pSNPs (
  id_added_pSNPs serial primary key,
  base_position int NOT NULL,
  template_base char(1) NOT NULL,
  pSNP_base char(1) NOT NULL,
  frequency smallint NOT NULL,
  start_tandom_repeat int
);

DROP TABLE IF EXISTS VarientListing;
CREATE TABLE VarientListing (
  id_VarientListingt serial primary key,
  read_id varchar(255) NOT NULL,
  read_position int NOT NULL,
  read_base char(1),
  template_positon int  NOT NULL,
  template_base char(1),
  phard smallint NOT NULL,
  ave_auality smallint,
  coverage smallint
);
