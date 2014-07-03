%%{

  machine termites_conf_core;

  WS = [\t\v\f ];    # whitespace
  EOL = '\r'? '\n';
  CONT = '\\' EOL;   # continuation

  def =  WS* ':' WS*;
  integer = digit+;
  word = alpha+;
  list = '(' >list_init WS* word >mark %list_append ( WS+ word >mark %list_append )* WS* ')';
  key_val = word >mark %key WS+ list;
  hash = ( key_val >hash_init %hash_insert ( WS+ key_val %hash_insert )* )**;
  coord = integer WS+ integer;

  # action attached to the previous rule/token
  temps_def    = 'temps'    def integer >mark %time_def;
  largeur_def  = 'largeur'  def integer >mark %width_def;
  hauteur_def  = 'hauteur'  def integer >mark %height_def;
  copeaux_def  = 'copeaux'  def list %chips_def;
  termites_def = 'termites' def hash %species_def;
  termite_def  = 'termite'  def word WS+ coord %{ FILE_LOG(logDEBUG) << "termite_def"; };
  copeau_def   = 'copeau'   def word WS+ coord %{ FILE_LOG(logDEBUG) << "copeau_def"; };

  line_comment = WS* '#' (any - EOL)* %{ FILE_LOG(logDEBUG) << "#"; };
  line_void    = WS* %{ FILE_LOG(logDEBUG) << "\\n"; };
  line_def     = WS* ( temps_def | largeur_def | hauteur_def | copeaux_def |
                       termites_def | termite_def | copeau_def ) WS*;

  line = ( line_comment | line_void | line_def ) EOL >line_count_inc;

  main := line+ $!error_any;

}%%
