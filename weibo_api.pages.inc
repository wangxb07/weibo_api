<?php

/**
 * @file page callback all in here
 */

/**
 * weibo oauth 2 callback page
 */
function weibo_api_oauth2_page($is_callback) {
  global $user;
  // process token fetch
  if (isset($_GET['code']) && $is_callback) {
    $client = weibo_api_build_oauth_client();

    $keys = array(
      'code' => $_GET['code'],
      'redirect_uri' => weibo_api_get_callback_uri(),
    );

    try {
      $token = $client->getAccessToken('code', $keys) ;
      weibo_api_token_set($token, TRUE);
    }
    catch (OAuthException $e) {
      drupal_set_message($e->getMessage(), 'error');
      drupal_goto();
    }

    if (isset($token['access_token'])) {
      $result = db_select('weibo_api_user', 'wu')
        ->fields('wu')
        ->condition('sina_uid', $token['uid'])
        ->execute()
        ->fetchObject();

      if ($user->uid) { // loggin
        if (isset($result->uid) && $result->uid != $user->uid) {
          drupal_set_message(t('This user exists'), 'error');
        }
        else {
          weibo_api_bind_user($token);
          drupal_set_message('bind success');
        }
        drupal_goto();
      }
      else { // not loggin
        if (isset($result->uid) && $result->uid > 0) {
          $form_state['uid'] = $result->uid;
        }
        else {
          $new_account = weibo_api_signup_user($token);
          $form_state['uid'] = $new_account->uid;
          drupal_set_message('bind success');
        }
        user_login_submit(array(), $form_state);
        drupal_goto('user/' . $form_state['uid']);
      }
    }
  }
  else {
    return array('#markup' => 'This page is weibo api oauth callback');
  }
}

/**
 * user edit bind page
 * @param object $user current page user
 */
function weibo_api_bind_page($user) {
  $output = array();

  if (!weibo_api_user_is_bind($user)) {
    $client = weibo_api_build_oauth_client();
    $login_uri = $client->getAuthorizeURL(weibo_api_get_callback_uri());
    $login_button = array(
      '#theme' => 'link',
      '#text' => t('Bind'),
      '#path' => $login_uri,
      '#options' => array(
        'attributes' => array(
        ),
        'html' => TRUE,
      ),
    );
    $output['button'] = $login_button;
  }
  else {
    $token = weibo_api_token_get();
    $client = weibo_api_build_client($token);
    $output['something'] = array('#markup' => 'hello world');
  }

  return $output;
}
