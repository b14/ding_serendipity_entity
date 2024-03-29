<?php
// $id$

/**
 * @file
 *  Serendipity entity ctools content_type
 */

$plugin = array(
  'title' => t('Ding serendipity ting entity'),
  'description' => t('Provide serendipity based on entities.'),
  'admin info' => 'ding_serendipity_ting_entity_admin_info',
  'single' => TRUE,
  'edit form' => 'ding_serendipity_ting_entity_serendipity_entity_content_type_edit_form',
  'render callback' => 'ding_serendipity_ting_entity_type_render',
  'category' => t('Ding!'),
);

/**
 * Returns a summary for the administration information in panels.
 */
function ding_serendipity_ting_entity_admin_info($subtype, $conf, $context) {
  $output = new stdClass();
  $output->title = t('Serendipity');
  $output->content = t('Serendipity content. View mode: @view_mode', array('@view_mode' => $conf['view_mode']));
  $output->content .= t('Sort by weight: @sort_weight', array('@sort_weight' => $conf['sort_weight']));
  $output->content .= t('Maximum items: @max', array('@max' => $conf['max']));
  $output->content .= t('Minimum items: @min', array('@min' => $conf['min']));
  $output->content .= t('Sort random: @random', array('@random' => $conf['random']));
  return $output;
}

/**
 * Implements hook_content_type_edit_form().
 */
function ding_serendipity_ting_entity_serendipity_entity_content_type_edit_form($form, &$form_state) {
  module_load_include('inc', 'ding_serendipity', 'plugins/content_types/serendipity');
  $form += ding_serendipity_form_base($form_state);
  $form['entity_type'] = array(
    '#type' => 'select',
    '#title' => t('Selected'),
    '#options' => array(
      'node' => t('Node'),
      'ting_object' => t('Ting object'),
    ),
    '#default_value' => (isset($form_state['conf']['entity_type'])) ? $form_state['conf']['entity_type'] : 'node',
    '#description' => t('Select the view mode which the content will display with.'),
  );
  $form['display'] = array(
    '#type' => 'select',
    '#title' => t('Selected'),
    '#options' => array(
      'content' => t('Site content'),
      'material' => t('Material'),
    ),
    '#default_value' => (isset($form_state['conf']['display'])) ? $form_state['conf']['display'] : 'content',
    '#description' => t('Select the type of content you wish to display.'),
  );
  $form['limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Serendipity limit'),
    '#description' => t('The maximum number of items to display in the initial list.'),
    '#default_value' => (isset($form_state['conf']['limit'])) ? $form_state['conf']['limit'] : 4,
    '#size' => 2,
    '#maxlength' => 2,
  );
  return $form;
}

/**
 * Implements hook_content_type_edit_form_validate().
 */
function ding_serendipity_ting_entity_serendipity_entity_content_type_edit_form_validate($form, &$form_state) {
  
}

/**
 * Implements hook_content_type_edit_form_submit().
 */
function ding_serendipity_ting_entity_serendipity_entity_content_type_edit_form_submit($form, &$form_state) {
  $form_state['conf']['view_mode'] = isset($form_state['values']['view_mode']) ? $form_state['values']['view_mode'] : 'big_teaser';
  $form_state['conf']['sort_weight'] = isset($form_state['values']['sort_weight']) ? $form_state['values']['sort_weight'] : FALSE;
  $form_state['conf']['max'] = isset($form_state['values']['max']) ? $form_state['values']['max'] : 3;
  $form_state['conf']['min'] = isset($form_state['values']['min']) ? $form_state['values']['min'] : 3;
  $form_state['conf']['random'] = isset($form_state['values']['random']) ? $form_state['values']['random'] : FALSE;
  $form_state['conf']['entity_type'] = $form_state['values']['entity_type'];
  $form_state['conf']['bundle'] = ($form_state['conf']['entity_type'] == 'ting_object') ? 'ting_object' : '';
  $form_state['conf']['limit'] = $form_state['values']['limit'];
  $form_state['conf']['display'] = $form_state['values']['display'];
}


function ding_serendipity_ting_entity_type_render($subtype, $conf, $args, $context) {
  if ($conf['entity_type'] == 'node') {
    $node = node_load($args[0]);
    $conf['bundle'] = $node->type;
  }
  $context = array(
    'entity_type' => $conf['entity_type'],
    'bundle' => $conf['bundle'],
    'node_nid' => $args[0],
    'view_mode' => isset($conf['view_mode']) ? $conf['view_mode'] : 'big_teaser',
    'sort_weight' => isset($conf['sort_weight']) ? $conf['sort_weight'] : FALSE,
    'max' => isset($conf['max']) ? $conf['max'] : 3,
    'min' => isset($conf['min']) ? $conf['min'] : 3,
    'random' => isset($conf['random']) ? $conf['random'] : FALSE,
  );
  $context[$conf['display']] = TRUE;
  $limit = isset($conf['limit']) ? $conf['limit'] : 4;
  $content = ding_serendipity_render($context);
  $output = array();
  if (!empty($content)) {
    $output['content'] = $content;
  }
  $block = new stdClass();
  $block->title = ($conf['display'] == 'material') ? t('Related material') : t('Related content');
  $block->content = $output;
  return $block;
}
