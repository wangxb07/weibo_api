<?php

/**
 * @file weibo api admin pages callback
 */

/**
 * settings form, disable/enable input, consumer_key, consumer_secret
 */
function weibo_api_settings_form(&$form_state) {
  // todo
  $form['weibo_api_is_enable'] = array(
    '#type' => 'radios',
    '#title' => t('Allow weibo connector'),
    '#default_value' => variable_get('weibo_api_is_enable', TRUE),
    '#options' => array(
      t('disable'),
      t('enable'),
    ),
  );

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Api Settings'),
    '#states' => array(
      'visible' => array(
        ':input[name="weibo_api_is_enable"]' => array('value' => '1'),
      ),
    ),
    '#tree' => FALSE,
  );

  $form['settings']['weibo_api_oauth_consumer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer key'),
    '#default_value' => variable_get('weibo_api_oauth_consumer_key', ''),
  );

  $form['settings']['weibo_api_oauth_consumer_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer secret'),
    '#default_value' => variable_get('weibo_api_oauth_consumer_secret', ''),
  );

  $form['settings']['weibo_api_callback_point'] = array(
    '#type' => 'textfield',
    '#title' => t('Callback point'),
    '#default_value' => variable_get('weibo_api_callback_point', 'callback'),
  );

  $form['settings']['weibo_login_is_enable'] = array(
    '#type' => 'radios',
    '#title' => t('Is enable weibo login'),
    '#default_value' => variable_get('weibo_login_is_enable', TRUE),
    '#options' => array(
      t('disable'),
      t('enable'),
    ),
  );

  return system_settings_form($form);
}