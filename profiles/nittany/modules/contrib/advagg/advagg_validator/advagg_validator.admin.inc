<?php

/**
 * @file
 * Admin page callbacks for the advagg validator module.
 */

/**
 * Form builder; Configure advagg settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function advagg_validator_admin_settings_form() {
  drupal_set_title(t('AdvAgg: Validator'));
  $config_path = advagg_admin_config_root_path();

  $form = array();
  $form['submit_advagg_css'] = array(
    '#type' => 'submit',
    '#value' => t('Run All root CSS files found in AdvAgg through the jigsaw.w3.org validator'),
    '#submit' => array('advagg_validator_admin_test_advagg_css_submit'),
    "#ajax" => array(
      "callback" => "advagg_validator_admin_test_advagg_css_callback",
      "wrapper" => 'advagg-validator-css-validator-ajax',
      "effect" => 'fade',
    ),
  );
//   $form['submit_all_css'] = array(
//     '#type' => 'submit',
//     '#value' => t('Run All CSS files found inside of the drupal webroot through the jigsaw.w3.org validator'),
//     '#submit' => array('advagg_validator_admin_test_all_css_submit'),
//     "#ajax" => array(
//       "callback" => "advagg_validator_admin_test_all_css_callback",
//       "wrapper" => 'advagg-validator-css-validator-ajax',
//       "effect" => 'fade',
//     ),
//   );
  $form["wrapper"] = array(
    "#markup" => "<div id='advagg-validator-css-validator-ajax'></div>",
  );

  return $form;
}

// Submit callbacks.
/**
 * Display file info in a drupal message.
 */
function advagg_validator_admin_test_advagg_css_submit($form, &$form_state) {
  module_load_include('inc', 'advagg_validator', 'advagg_validator');

  if (!empty($form_state['input']['ajax_page_state'])) {
    return;
  }
  $info = advagg_validator_test_advagg_css();
  $info = advagg_validator_admin_hide_good_files($info);
  if (module_exists('httprl')) {
    $output = httprl_pr($info);
  }
  else {
    $output = '<pre>' . print_r($info, TRUE) . '</pre>';
  }
  drupal_set_message($output);
}

/**
 * Display file info via ajax callback.
 */
function advagg_validator_admin_test_advagg_css_callback($form, &$form_state) {
  module_load_include('inc', 'advagg_validator', 'advagg_validator');

  if (!empty($form_state['values']['error'])) {
    return '<div id="advagg-file-info-ajax"></div>';
  }
  $info = advagg_validator_test_advagg_css();
  if (empty($info)) {
    return '<div id="advagg-file-info-ajax"></div>';
  }
  $info = advagg_validator_admin_hide_good_files($info);
  if (module_exists('httprl')) {
    $output = httprl_pr($info);
  }
  else {
    $output = '<pre>' . print_r($info, TRUE) . '</pre>';
  }
  return '<div id="advagg-validator-css-validator-ajax">' . $output . '</div>';
}

/**
 * Display file info in a drupal message.
 */
function advagg_validator_admin_test_all_css_submit($form, &$form_state) {
  module_load_include('inc', 'advagg_validator', 'advagg_validator');

  if (!empty($form_state['input']['ajax_page_state'])) {
    return;
  }
  $info = advagg_validator_test_all_css();
  $info = advagg_validator_admin_hide_good_files($info);
  if (module_exists('httprl')) {
    $output = httprl_pr($info);
  }
  else {
    $output = '<pre>' . print_r($info, TRUE) . '</pre>';
  }
  drupal_set_message($output);
}

/**
 * Display file info via ajax callback.
 */
function advagg_validator_admin_test_all_css_callback($form, &$form_state) {
  module_load_include('inc', 'advagg_validator', 'advagg_validator');

  if (!empty($form_state['values']['error'])) {
    return '<div id="advagg-file-info-ajax"></div>';
  }
  $info = advagg_validator_test_all_css();
  if (empty($info)) {
    return '<div id="advagg-file-info-ajax"></div>';
  }
  $info = advagg_validator_admin_hide_good_files($info);
  if (module_exists('httprl')) {
    $output = httprl_pr($info);
  }
  else {
    $output = '<pre>' . print_r($info, TRUE) . '</pre>';
  }
  return '<div id="advagg-validator-css-validator-ajax">' . $output . '</div>';
}

function advagg_validator_admin_hide_good_files($info) {
  $output = array();
  foreach ($info as $filename => $validators) {
    foreach ($validators as $v_name => $v_data) {
      if (!empty($v_data['validity'])) {
        $output[$filename][$v_name] = 'valid';
        continue;
      }
      $output[$filename][$v_name] = $v_data;
    }
  }
  return $output;
}