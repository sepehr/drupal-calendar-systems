DESCRIPTION
===========
Support for various calendars like Jalali, Gregorian, Hijir, Hebew, etc.

INSTALLATION
============
  - Install and enable the module as usual: http://drupal.org/node/70151
  - Add the following code right after "$timezones = &$drupal_static_fast['timezones'];" in "/includes/common.inc" file
    or apply the patch located in patches/ in module's directory. if you don't know how to apply patches, there is a good guide here:
    http://drupal.org/patch/apply

          foreach (module_implements('format_date') AS $module) {
            if ($module!='date') {
                $function = $module .'_format_date';
                $r=$function($timestamp, $type, $format, $timezone, $langcode);

                if ($r!=false) {
                    return $r;
                }
            }
          }

  - Goto "admin/config/regionals/calendar-systems" and configure your profiles.

SUPPORT
=======
Hunted a bug? report here: http://drupal.org/node/add/project-issue/calendar_systems

AUTHORS AND MAINTAINERS
=======================
  Sina Salek - Original developer.
  Sepehr Lajevardi - D7 developer.
