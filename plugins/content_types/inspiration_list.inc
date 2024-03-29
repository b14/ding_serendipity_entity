<?php
// $id$

/**
 * @file
 *  Serendipity inspiration list ctools content_type
 */

$plugin = array(
  'title' => t('General purpose ding_object serendipity panel'),
  'description' => t('Provide serendipity based a taxonomy term.'),
  'admin info' => 'ding_serendipity_entity_inspiration_list_admin_info',
  'single' => TRUE,
  'edit form' => 'ding_serendipity_entity_inspiration_list_content_type_edit_form',
  'render callback' => 'ding_serendipity_entity_inspiration_list_content_type_render',
  'category' => t('Ding!'),
);

/**
 * Returns a summary for the administration information in panels.
 */
function ding_serendipity_entity_inspiration_list_admin_info($subtype, $conf, $context) {
  $output = new stdClass();
  $output->title = t('Serendipity inspiration list');
  $output->content = t('Serendipity content. View mode: @view_mode', array('@view_mode' => $conf['view_mode'])) . '<br/>';
  $output->content .= t('Sort by weight: @sort_weight', array('@sort_weight' => $conf['sort_weight'])) . '<br/>';
  $output->content .= t('Maximum items: @max', array('@max' => $conf['max'])) . '<br/>';
  $output->content .= t('Minimum items: @min', array('@min' => $conf['min'])) . '<br/>';
  $output->content .= t('Sort random: @random', array('@random' => $conf['random'])) . '<br/>';
  return $output;
}

/**
 * Implements hook_content_type_edit_form().
 */
function ding_serendipity_entity_inspiration_list_content_type_edit_form($form, &$form_state) {
  module_load_include('inc', 'ding_serendipity', 'plugins/content_types/serendipity');
  $base = ding_serendipity_form_base($form_state);
  $form += $base;
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
 * Implements hook_content_type_edit_form_submit().
 */
function ding_serendipity_entity_inspiration_list_content_type_edit_form_submit($form, &$form_state) {
  ding_serendipity_form_base_submit($form_state);
  $form_state['conf']['limit'] = $form_state['values']['limit'];
}


function ding_serendipity_entity_inspiration_list_content_type_render($subtype, $conf, $args, $context) {
  $context = array(
    'entity_type' => 'ding_object',
    'bundle' => 'ding_object',
    'ting_object_id' => $args[0],
    'view_mode' => !empty($conf['view_mode']) ? $conf['view_mode'] : 'big_teaser',
    'sort_weight' => !empty($conf['sort_weight']) ? $conf['sort_weight'] : FALSE,
    'max' => !empty($conf['max']) ? $conf['max'] : 4,
    'min' => !empty($conf['min']) ? $conf['min'] : 4,
    'random' => !empty($conf['random']) ? TRUE : FALSE,
  );
  $limit = !empty($conf['limit']) ? $conf['limit'] : 4;
  $content = array(
    '#theme' => 'ding_tabs',
    '#name' => 'related_material',
    '#title' => t('Related material'),
    '#navigation' => 'My navigation',
    '#tabs' => array(
      'object' => array(
        '#title' => 'Related material',
        '#theme' => 'ding_tabs_content',
        '#type' => 'serendipity',
        '#ajax' => TRUE,
        '#options' => array(
          'context' => $context,
        ),
        '#limit' => $limit,
      ),
    ),
  );
  $output['content'] = $content;

  $block = new stdClass();
  $block->title = t('Related material');
  $block->content = $output;
  return $block;
}
