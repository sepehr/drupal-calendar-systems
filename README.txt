--- README  -------------------------------------------------------------

Calendar Systems

Written by Sina Salek
  http://sina.salek.ws

Requirements: Drupal 6.x



--- INSTALLATION --------------------------------------------------------

1. Install module, if you don't know how to do it , read this : http://drupal.org/node/70151

2. Add the following code at right after "function format_date($timestamp, $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL) {" 
in "/includes/common.inc" or run the patch file located in patches/ folder in module's folder. if you don't know how to apply a patch,
there is a very good tutorial here http://drupal.org/patch/apply

  // Custom hook
  foreach (module_implements('format_date') AS $module) {
    if ($module!='date') {
        $function = $module .'_format_date';
        $r=$function($timestamp, $type, $format, $timezone, $langcode);
    
        if ($r!=false) {
            return $r;
        }
    }
  }

3. Enable "Calendar Systems" under Administer > Site building > Modules.

4. GoTo "Administer -> Site Configuration -> Calendar Systems" and assign calendar systems to different languages

5. Change interface's language, you should be able to see date(s) in the assinged calendar system's format

--- Support --------------------------------------------------------

Found a bug? report it here http://drupal.org/node/add/project-issue/calendar_systems