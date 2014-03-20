<?php

/**
 * @file
 * Administration page callbacks for the ad module.
 */
/**
 *
 * @ingroup forms
 * @see system_settings_form().
 */

/**
 * Menu callback for admin/content/ndv_exchange.
 */
function antimate_admin_content() {
    $words = db_select('antimate_words', 't')
            ->fields('t')
            ->execute()
            ->fetchAllAssoc('id');
    return theme('antimate_admin_content', array('words' => $words));
}

function antimate_admin_settings() {

    $form['antimate_word'] = array(
        '#type' => 'textfield',
        '#title' => t('Test'),
        '#default_value' => '',
    );
    $form['#submit'][] = 'antimate_admin_settings_submit';

    return system_settings_form($form);
}

function antimate_admin_settings_submit($form, $form_state) {
    $text = $form_state['values']['antimate_word'];
    if (!empty($text)) {
        db_merge('antimate_words')
                ->key(array('text' => $text))
                ->fields(array('text' => $text, 'uid' => $GLOBALS['user']->uid)
        );
    }
}


function theme_antimate_admin_content($vars) {
    $words = $vars['words'];
    if ($words) {
        $header = array(
            t('Text'),
            t('Type'),
            '',
        );
        $rows = array();
        foreach ($words AS $item) {
            switch($item->type){
                case 'badword_reg': 
                    $type = t('`Bad` word regular expression');
                    break;
                case 'exclusion':
                    $type = t('Word, excluded from filter');
                    break;
                case 'exclusion_reg':
                    $type = t('Regular expression, excluded from filter');
                    break;
                default: 
                    $type = t('`Bad` word');
                    break;
            }
            $rows[] = array(
                $item->text,
                $type,
                l(t('Delete'),'admin/content/antimate/'.$item->id.'/delete'),
            );
        }
        return theme('table', array('header' => $header, 'rows' => $rows));
    }
    else
        return t('There is nothing to render');
}

?>