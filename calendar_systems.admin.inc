<?php

/**
 * @file
 * Integrate Calendar systems editors into Drupal.
 */

/**
 * Form callback for calendar systems profiles.
 *
 * @ingroup forms
 */
function calendar_systems_profile_overview($form, &$form_state) {
  $form = array();

  // Do not show the form in case that the patch is not yet applied.
  if (!_calendar_systems_patch_applied()) {
    // Build a form item, indicating the problem:
    $form['calendar_systems_error'] = array(
      '#type' => 'value',
      '#value' => t('You did not apply the required core patch for calendar systems to work, please follow the instructions in <a href="!readme-link">README.txt</a> file.', array(
        '!readme-link' => url(drupal_get_path('module', 'calendar_systems') . '/README.txt'),
      )),
    );
    // Render:
    return $form;
  }

  // The patch is properly applies,
  // build the overview form array.
  $form['formats'] = array();
  $form['formats']['#tree'] = TRUE;

  // Set initials.
  $options = array();
  $profiles  = _calendar_systems_profiles();
  $languages = _calendar_systems_langauges();
  $calendar_systems = _calendar_systems_plugins();

  // Build available calendar systems array:
  foreach ($calendar_systems as $id => $calendar_system) {
    $options[$id] = $calendar_system['title'];
  }

  // Table rows:
  foreach ($languages as $id => $language) {
    // Language:
    $form['formats'][$id]['name'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($language['name']),
    );
    // Calendar systems:
    $form['formats'][$id]['editor'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#id' => "edit-editor-$id",
      '#default_value' => isset($profiles[$id]) ? $profiles[$id]->calendar_system : '',
      '#disabled' => isset($profiles[$id]) ? (bool) $profiles[$id]->calendar_system : FALSE,
    );
    // Operation links:
    if (isset($profiles[$id]) && !empty($profiles[$id]->calendar_system)) {
      $form['formats'][$id]['remove'] = array(
        '#type' => 'markup',
        '#markup' => l(t('Remove'), "admin/config/regional/calendar-systems/profile/$id/delete"),
      );
    }
  }
  // Append submit button:
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );

  // Render:
  return $form;
}

/**
 * Theme callback for calendar systems profiles form.
 */
function theme_calendar_systems_profile_overview($variables) {//dpm($variables['form']);dpm(element_children($variables['form']['formats']));return;
  // Check of unexpectency!
  if (!isset($variables['form']['formats'])) {
    return t('There were a problem getting calendar systems profiles.');
  }

  // Theme the form as a HTML table:
  $rows = array();
  // Build rows array:
  foreach (element_children($variables['form']['formats']) as $item) {
    $format = &$variables['form']['formats'][$item];
    $rows[] = array(
      drupal_render($format['name']),
      drupal_render($format['editor']),
      isset($format['remove']) ? drupal_render($format['remove']) : '',
    );
  }
  // And the header:
  $header = array(
    t('Language'),
    t('Calendar system'),
    array(
      'colspan' => 2,
      'data' => t('Operations'),
    )
  );
  // Theme the table:
  $output = theme('table', array('header' => $header, 'rows' => $rows));
  // Render the actual form & append:
  $output .= drupal_render_children($variables['form']);
  return $output;
}

/**
 * Submission callback for calendar systems profiles form.
 */
function calendar_systems_profile_overview_submit($form, &$form_state) {
  foreach ($form_state['values']['formats'] as $format => $values) {
    if ($values['editor'] != 'default') {
      // Build the record array:
      $record = array(
        'language' => $format,
        'calendar_system' => $values['editor'],
      );
      // Try to update existing profile:
      // drupal_write_record() returns SAVED_UPDATED even if there are no records to update!
      if (!db_update('calendar_systems')->fields($record)->condition('language', $format)->execute()) {
        // Insert otherwise:
        drupal_write_record('calendar_systems', $record);
      }
    }
  }
  // Notify user:
  drupal_set_message(t('Calendar systems profile configuration has been saved.'));
}

/**
 * Form callback for profile removal confirmarion.
 *
 * @ingroup forms
 */
function calendar_systems_profile_delete_confirm($form, &$form_state, $profile) {
  $languages = calendar_systems_get_all_langauges();
  $language = $languages[$profile];

  // Build form array:
  $form = array();
  $form['format'] = array(
    '#type' => 'value',
    '#value' => $language['name']
  );
  // Create the confirmation
  // form and let it be rendered:
  return confirm_form(
    $form,
    t('Are you sure you want to remove the profile for %name?', array('%name' => $language['name'])),
    'admin/config/regional/calendar-systems',
    t('This action cannot be undone.'), t('Remove'), t('Cancel')
  );
}

/**
 * Submission callback for profile removal confirmarion form.
 */
function calendar_systems_profile_delete_confirm_submit($form, &$form_state) {
  // Delete the specified profile:
  db_delete('calendar_systems')
    ->condition('language', $form_state['values']['format'])
    ->execute();
  // Notify user:
  drupal_set_message(t('Calendar systems profile for %name has been deleted.',
    array('%name' => $form_state['values']['format'])
  ));
  // Redirect user to the profile overview form:
  $form_state['redirect'] = 'admin/config/regional/calendar-systems';
}

/**
 * Internal helper to check whether the required patch is applied or not.
 *
 * @return
 *   Boolean value.
 */
function _calendar_systems_patch_applied() {
  $content = file_get_contents(DRUPAL_ROOT . '/includes/common.inc');
  // Check against patch fingerprint:
  return (strpos($content,'foreach (module_implements(\'format_date\') AS $module) {') !== FALSE) ?
    TRUE : FALSE;
}
