<?php
/**
 * Player position delete confirm form.
 *
 * @author Rebecca Krish
 */
function sams_player_position_form_delete_confirm($form, &$form_state, $player_position) {
  $form_state['player_position'] = $player_position;

  $form['player_position'] = array(
    '#type' => 'value',
    '#value' => entity_id('sams_player_position', $player_position),
  );

  return confirm_form($form,
    t('Are you sure you want to delete equipment type %title?', array('%title' => entity_label('sams_player_position', $player_position))),
    'equipment/' . entity_id('sams_player_position', $player_position),
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}


/**
 * Player position delete submit handler.
 *
 * @author Rebecca Krish
 */
function sams_player_position_form_delete_confirm_submit($form, &$form_state) {
  $player_position = $form_state['player_position'];
  sams_player_position_delete($player_position);

  watchdog('sams_player_position', 'Deleted %title.', array('%title' => $player_position->label));
  drupal_set_message(t('Position %title has been deleted.', array('%title' => $player_position->label)));

  $form_state['redirect'] = 'admin/sams/positions';
}


/**
 * Form builder; sams_player_position entity
 *
 * Display a form to edit a sams_player_position entity, as required by the
 * Entity Admin UI.
 *
 * @author Rebecca Krish
 */
function sams_player_position_form($form, &$form_state, $player_position, $op = 'edit') {
  $form_state['player_position'] = $player_position;

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => empty($player_position->title) ? '' : $player_position->title,
    '#required' => TRUE,
    '#weight' => -10,
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => empty($player_position->description) ? '' : $player_position->description,
    '#required' => TRUE,
    '#weight' => -8,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
    '#weight' => 20,
  );
  $form['cancel'] = array(
    '#type' => 'link',
    '#href' => 'admin/sams/positions',
    '#title' => t('Cancel'),
    '#weight' => 21,
  );

  return $form;
}


/**
 * Submit handler for sams_player_position_form().
 *
 * @author Rebecca Krish
 */
function sams_player_position_form_submit($form, &$form_state) {
  $player_position = $form_state['player_position'];

  entity_form_submit_build_entity('sams_player_position', $player_position, $form, $form_state);

  try {
    sams_player_position_save($player_position);
    drupal_set_message(t('Saved player position %position.', array('%position' => $player_position->title)));
    $form_state['redirect'] ='admin/sams/positions';
  }
  catch (Exception $e) {
    drupal_set_message(t('Encountered an error when saving the player position: ' . $e->getMessage()), 'error');
    return;
  }
}
