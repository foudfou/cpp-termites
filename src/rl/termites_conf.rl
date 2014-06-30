%%{

  machine termites_conf_core;

  ws = [\t\v\f ];    # whitespace
  eol = '\r'? '\n';
  cont = '\\' eol;   # continuation

  def =  ws* ':' ws*;
  integer = digit+;
  word = alpha+;
  list = '(' ws* word ( ws+ word )* ws* ')';
  key_val = word ws+ list;
  hash =  key_val ( ws+ key_val )* ;
  coord = integer ws+ integer;

  # action attached to the previous rule/token
  temps_def    = 'temps'    def integer >start_value %write_value;
  largeur_def  = 'largeur'  def integer %{ FILE_LOG(logDEBUG) << "largeur_def"; };
  hauteur_def  = 'hauteur'  def integer %{ FILE_LOG(logDEBUG) << "hauteur_def"; };
  copeaux_def  = 'copeaux'  def list %{ FILE_LOG(logDEBUG) << "copeaux_def"; };
  termites_def = 'termites' def hash %{ FILE_LOG(logDEBUG) << "termites_def"; };
  termite_def  = 'termite'  def word ws+ coord %{ FILE_LOG(logDEBUG) << "termite_def"; };
  copeau_def   = 'copeau'   def word ws+ coord %{ FILE_LOG(logDEBUG) << "copeau_def"; };

  line_comment = ws* '#' (any - eol)* %{ FILE_LOG(logDEBUG) << "#"; };
  line_void    = ws* %{ FILE_LOG(logDEBUG) << "\\n"; };
  line_def     = ws* ( temps_def | largeur_def | hauteur_def | copeaux_def |
                       termites_def | termite_def | copeau_def ) ws*;

  line = ( line_void | line_comment | line_def ) eol >line_count_inc;

  main := ( line )* <>!error_any;

}%%
