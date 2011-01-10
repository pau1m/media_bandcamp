<?php

// $Id: emfield.php,v 1.1.2.3 2009/10/29 18:29:43 aaron Exp $

/**
 * @file
 *   This include processes bandcamp media files for use by emfield.module.
 */
//should probably use an options field for user set options
//have one entire funcion which assigns all the variable gets to an options
//field. Could take away and replace $autoplay -> don't see how it could be needed here
//@TODO place comments around tricky code
//@TODO aa thumbnail view
// @TODO check the dev version of emfield. Seem to be several differences
// in the hooks implemented by other providers compared to those in hooks.php

/**
 *  Bandcamp URL
 */
define('EMAUDIO_BANDCAMP_MAIN_URL', 'http://www.bandcamp.com');

/**
 *  Define player bandcamp color, type, dimensions and default
 */
//color
define('EMAUDIO_BANDCAMP_BACKGROUND_COLOR_DEFAULT', 'FFFFFF');
define('EMAUDIO_BANDCAMP_LINK_COLOR_DEFAULT', '4285BB');

//type and dimensions
define('EMAUDIO_BANDCAMP_SIZE_VENTI', 'venti');
define('EMAUDIO_BANDCAMP_SIZE_VENTI_WIDTH', '400');
define('EMAUDIO_BANDCAMP_SIZE_VENTI_HEIGHT', '100');

define('EMAUDIO_BANDCAMP_SIZE_GRANDE', 'grande');
define('EMAUDIO_BANDCAMP_SIZE_GRANDE_WIDTH', '300');
define('EMAUDIO_BANDCAMP_SIZE_GRANDE_HEIGHT', '100');

define('EMAUDIO_BANDCAMP_SIZE_GRANDE2_TRACK', 'grande2');
define('EMAUDIO_BANDCAMP_SIZE_GRANDE2_WIDTH', '300');
define('EMAUDIO_BANDCAMP_SIZE_GRANDE2_HEIGHT', '355');

define('EMAUDIO_BANDCAMP_SIZE_GRANDE3_TRACK_ART', 'grande3');
define('EMAUDIO_BANDCAMP_SIZE_GRANDE3_WIDTH', '300');
define('EMAUDIO_BANDCAMP_SIZE_GRANDE3_HEIGHT', '415');

define('EMAUDIO_BANDCAMP_SIZE_TALL', 'tall');
define('EMAUDIO_BANDCAMP_SIZE_TALL_WIDTH', '150');
define('EMAUDIO_BANDCAMP_SIZE_TALL_HEIGHT', '295');

define('EMAUDIO_BANDCAMP_SIZE_TALL2_TRACK', 'tall2');
define('EMAUDIO_BANDCAMP_SIZE_TALL2_WIDTH', '300');
define('EMAUDIO_BANDCAMP_SIZE_TALL2_HEIGHT', '450');

define('EMAUDIO_BANDCAMP_SIZE_SHORT', 'short');
define('EMAUDIO_BANDCAMP_SIZE_SHORT_WIDTH', '46');
define('EMAUDIO_BANDCAMP_SIZE_SHORT_HEIGHT', '23');

//visualization
define('EMAUDIO_BANDCAMP_VISUALIZATION_BUBBLES', 'bubbles');
define('EMAUDIO_BANDCAMP_VISUALIZATION_ABSTRACT3D', 'abstract3d');
define('EMAUDIO_BANDCAMP_VISUALIZATION_FLOWERS', 'flowers');
define('EMAUDIO_BANDCAMP_VISUALIZATION_PLASMA', 'plasma');
define('EMAUDIO_BANDCAMP_VISUALIZATION_EQUALIZER', 'equalizer');
define('EMAUDIO_BANDCAMP_VISUALIZATION_HYPNO', 'hypno');
define('EMAUDIO_BANDCAMP_VISUALIZATION_PLUGHOLE', 'plughole');
define('EMAUDIO_BANDCAMP_VISUALIZATION_STRATA', 'strata');
define('EMAUDIO_BANDCAMP_VISUALIZATION_RIBBONS', 'ribbons');

//player defaults
define('EMAUDIO_BANDCAMP_ALBUM_SIZE_DEFAULT', 'grande2');
define('EMAUDIO_BANDCAMP_TRACK_SIZE_DEFAULT', 'venti');
define('EMAUDIO_BANDCAMP_SIZE_DEFAULT', 'venti');
define('EMAUDIO_BANDCAMP_VISUALIZATION_DEFAULT', 'none');


/**
 *  This defines the version of the content data array that we serialize
 *  in emvideo_hulu_data(). If we change the expected keys of that array,
 *  we must increment this value, which will allow older content to be updated
 *  to the new version automatically.
 */
define('EMAUDIO_BANDCAMP_DATA_VERSION', 1);

/**
 *  Return the information about a specific provider.
 *
 *  Each provider module must implement this hook to be recognized.
 *
 *  @return
 *    An associative array with the following information:
 *      'provider' => The machine name, should be same as the provider filename.
 *      'name' => The provider's human readable name.
 *      'url' => The URL to the provider's main page.
 *      'settings_description' => This will be displayed above the provider
 *        information on the administration page.
 *      'supported_features' => An array of cells to be passed to a table
 *        on the administration page, under the headers of 'Feature',
 *        'Supported', and 'Notes'.
 */
function emaudio_bandcamp_info() {
    $features = array(
        array(t('Autoplay'), t('no'), ''),
        array(t('RSS Attachment'), t('no'), ''),
        array(t('Thumbnails'), t('no'), t('')),
        array(t('Duration'), t('no'), ''),
        array(t('Full screen mode'), t('no'), t('You may customize the player to enable or disable full screen playback. Full screen mode is enabled by default.')),
        array(t('Use JW FLV Media Player'), t('no'), t("You may opt to use the !flvplayer to play example.com videos if it's installed on your server.", array('!flvplayer' => l(t('JW FLV Media Player'), 'http://www.longtailvideo.com/players/jw-flv-player/')))),
    );
    return array(
        'provider' => 'bandcamp',
        'name' => t('bandcamp'),
        'url' => EMAUDIO_BANDCAMP_MAIN_URL,
        'settings_description' => t('These settings specifically affect videos displayed from !bandcamp.', array('!bandcamp' => l(t('Bandcamp.com'), EMAUDIO_BANDCAMP_MAIN_URL))),
        'supported_features' => $features,
    );
}

/**
 *  Allow a provider to add its own elements to the content creation form.
 *
 * !!!!!!! have changed name of this function from
 * !!!!!!! emaudio_bandcamp_form($items, $field) because it didn't seem to be working
 * !!!!!!! come back and look at this
 *
 *
 */
function emaudio_bandcamp_settings() {
    $form['bandcamp']['player'] = array(
        '#type' => 'fieldset',
        '#title' => 'Player options',
        '#description' => 'Styling and playback options for the embedded player.',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['bandcamp']['player']['color'] = array(
        '#type' => 'fieldset',
        '#title' => t('Color'),
        '#description' => 'If allowed, these colors in hexidecimal RRGGBB (without #), will be used to change the skin of the Bandcamp player.',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['bandcamp']['player']['color']['emaudio_bandcamp_color_override'] = array(
        '#type' => 'checkbox',
        '#title' => 'Override player color',
        '#default_value' => variable_get('emaudio_bandcamp_color_override', FALSE),
    );

    // find an example using the colour wheel module

    $form['bandcamp']['player']['color']['emaudio_bandcamp_background_color'] = array(
        '#type' => 'textfield',
        '#title' => t('Background Color'),
        '#default_value' => variable_get('emaudio_bandcamp_background_color', EMAUDIO_BANDCAMP_BACKGROUND_COLOR_DEFAULT),
    );

    $form['bandcamp']['player']['color']['emaudio_bandcamp_link_color'] = array(
        '#type' => 'textfield',
        '#title' => 'Text Color',
        '#default_value' => variable_get('emaudio_bandcamp_link_color', EMAUDIO_BANDCAMP_LINK_COLOR_DEFAULT),
    );

    $form['bandcamp']['player']['dimensions'] = array(
        '#type' => 'fieldset',
        '#title' => 'Size and Type',
        '#description' => 'Set the size and type of the player for albums and tracks',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );

    $form['bandcamp']['player']['dimensions']['emfield_bandcamp_override_default_dimensions'] = array(
        '#type' => 'checkbox',
        '#title' => 'Override emfield default sizes',
        '#description' => 'Use the sizes set here in place of emaudio defaults',
        '#default_value' => 1,
    );


    $form['bandcamp']['player']['dimensions']['album']['emaudio_bandcamp_album_full_dimensions'] = array(
        '#type' => 'select',
        '#title' => 'Album (full display)',
        '#default_value' => variable_get('emaudio_bandcamp_album_full_dimensions', EMAUDIO_BANDCAMP_ALBUM_SIZE_DEFAULT),
        '#options' => array(
            'venti' => t('Venti (400 x 100)'),
            'grande' => t('Grande (300 x 100)'),
            'grande2' => t('Grande with tracklist (400 x 355)'),
            'grande3' => t('Grande with tracklist and art (300 x 415)'),
            'tall' => t('Tall (150 x 295)'),
            'tall2' => t('Tall with tracklist (150 x 450)'),
            'short' => t('Short (46 x 23)'),
        ),
        '#description' => 'Set the type and size of embedded media player for albums',
    );



    $form['bandcamp']['player']['dimensions']['album']['emaudio_bandcamp_album_preview_dimensions'] = array(
        '#type' => 'select',
        '#title' => 'Album (preview)',
        '#default_value' => variable_get('emaudio_bandcamp_album_preview_dimensions', EMAUDIO_BANDCAMP_ALBUM_SIZE_DEFAULT),
        '#options' => array(
            'venti' => t('Venti (400 x 100)'),
            'grande' => t('Grande (300 x 100)'),
            'grande2' => t('Grande with tracklist (400 x 355)'),
            'grande3' => t('Grande with tracklist and art (300 x 415)'),
            'tall' => t('Tall (150 x 295)'),
            'tall2' => t('Tall with tracklist (150 x 450)'),
            'short' => t('Short (46 x 23)'),
        ),
        '#description' => 'Set the type and size of embedded media player for albums',
    );


    $form['bandcamp']['player']['dimensions']['track']['emaudio_bandcamp_track_full_dimensions'] = array(
        '#type' => 'select',
        '#title' => 'Track (full display)',
        '#default_value' => variable_get('emaudio_bandcamp_track_full_dimensions', EMAUDIO_BANDCAMP_TRACK_SIZE_DEFAULT),
        '#options' => array(
            'venti' => t('Venti (400 x 100)'),
            'grande' => t('Grande (300 x 100)'),
            'grande2' => t('Grande with tracklist (400 x 355)'),
            'grande3' => t('Grande with tracklist and art (300 x 415)'),
            'tall' => t('Tall (150 x 295)'),
            'tall2' => t('Tall with tracklist (150 x 450)'),
            'short' => t('Short (46 x 23)'),
        ),
        '#description' => 'Set the type and size of embedded media player for tracks',
    );

    $form['bandcamp']['player']['dimensions']['track']['emaudio_bandcamp_track_preview_dimensions'] = array(
        '#type' => 'select',
        '#title' => 'Track (preview)',
        '#default_value' => variable_get('emaudio_bandcamp_track_preview_dimensions', EMAUDIO_BANDCAMP_TRACK_SIZE_DEFAULT),
        '#options' => array(
            'venti' => t('Venti (400 x 100)'),
            'grande' => t('Grande (300 x 100)'),
            'grande2' => t('Grande with tracklist (400 x 355)'),
            'grande3' => t('Grande with tracklist and art (300 x 415)'),
            'tall' => t('Tall (150 x 295)'),
            'tall2' => t('Tall with tracklist (150 x 450)'),
            'short' => t('Short (46 x 23)'),
        ),
        '#description' => 'Set the type and size of embedded media player for tracks',
    );

    $form['bandcamp']['player']['visualization'] = array(
        '#type' => 'fieldset',
        '#title' => t('Visualization'),
        '#description' => t('Set the default visualization'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );

    $form['bandcamp']['player']['visualization']['emaudio_bandcamp_visualization'] = array(
        '#type' => 'radios',
        '#title' => 'Album',
        '#default_value' => variable_get('emaudio_bandcamp_visualization', EMAUDIO_BANDCAMP_VISUALIZATION_DEFAULT),
        '#options' => array(
            'none' => t('No visuals, just display the player'),
            'bubbles' => t('Bubbles'),
            'abstract3d' => t('Abstract 3D'),
            'flowers' => t('Flowers'),
            'plasma' => t('Plasma'),
            'equalizer' => t('Equalizer'),
            'hypno' => t('Hypno'),
            'plughole' => t('Plughole'),
            'strata' => t('Strata'),
            'ribbons' => t('Ribbons'),
        ),
        '#description' => 'Choose the default visualization',
    );

    return $form;
}

/**
 *  Parse the URL or embed code provided by an editor.
 *
 *  @param $embed
 *    The raw URL or embed code pasted into the text field by the editor.
 *  @param $field
 *    The field data for the emvideo, emaudio, etc.
 *  @return
 *    If the hook returns a non-empty string, then it is assumed to have been
 *    parsed and matched by this provider. If the hook returns an array of
 *    strings, then each string is assumed to be a regex pattern, and will
 *    be checked for a match in turn. Otherwise, it is assumed there is no
 *    match.
 */
function emaudio_bandcamp_extract($embed, $field) {


    return array(
        // matching urls
        // http://laurashigihara.bandcamp.com
        // http://laurashigihara.bandcamp.com/track/crazy-daves-greeting
        // http://laurashigihara.bandcamp.com/album/plants-vs-zombies-soundtrack
        '@((?:\w+)\.bandcamp\.com/(album|track)?/(?:[^"\&]+))@i',
    //@TODO match embed code
    );
}

/**
 *  Validate the provider's embedded code.
 *
 *  This allows each provider to determine whether a pasted URL that otherwise
 *  matches a parse attempt from EMMODULE_PROVIDER_extract is valid.
 *  For instance, the provider file might check against the third party
 *  provider's API to ensure a video still exists. If the validation is to
 *  fail, the function should call form_set_error with the provided
 *  $error_field.
 *
 *  @param $code
 *    The unique identifier for the third party media.
 *  @param $error_field
 *    The form field to use with form_set_error().
 */
function EMMODULE_PROVIDER_validate($code, $error_field) {
    if (!_example_call_api('video_data', $code)) {
        form_set_error($error_field, t('That video no longer exists on example.com.'));
    }
}

function emaudio_bandcamp_data($field, $item) {
    //@TODO capture the thumbnail as well so as it can be used in theming
    //@TODO add ability to handle embedded item as well
    //is this line necessary
    $data = array('emaudio_bandcamp_version' => EMAUDIO_BANDCAMP_DATA_VERSION);

    $response = drupal_http_request($item['embed']);
    if ($response->code == 200) {

        //extract 'album' or 'track' and unique numerical identifier
        preg_match('@(?:video_src.+)(album|track)=([\d]+)@i', $response->data, &$matches);

        $data['type'] = $matches[1]; // 'album' or 'track'
        $data['code'] = $matches[2]; // the numerical identifier for the album or track
        //extract preview image URL
        preg_match('@http://[^"\&]+\.jpg@i', $response->data, &$matches);
        $data['preview image'] = $matches[0];
    }



    return $data;
}

function emaudio_bandcamp_data_version($field, $item) {
    return EMAUDIO_BANDCAMP_DATA_VERSION;
}

/**
 * The embedded flash displaying the SoundCloud audio.
 */
//function theme_emaudio_bandcamp_flash($item, $embed, $width, $height, $autoplay) {
function theme_emaudio_bandcamp_flash($item) {

    $type = $item['data']['type'];
    $code = $item['data']['code'];
    $width = $item['options']['width'];
    $height = $item['options']['height'];
    $size = $item['options']['size'];
    $bg_color = $item['options']['bg_color'];
    $link_color = $item['options']['link_color'];

    //don't forget to add a bit about visualization

    $output = <<<BC
<object
data="http://bandcamp.com/EmbeddedPlayer/{$type}={$code}/size={$size}/bgcol={$bg_color}/linkcol={$link_color}//"
type="text/html" classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" width="{$width}" height="{$height}">
<param name="movie" value="http://bandcamp.com/EmbeddedPlayer/{$type}={$code}/size={$size}/bgcol={$bg_color}/linkcol={$link_color}//">
<param name="quality" value="high">
<param name="allowNetworking" value="always">
<param name="wmode" value="transparent">
<param name="bgcolor" value="#{$bg_color}">
<param name="allowScriptAccess" value="never">
<object data="http://bandcamp.com/EmbeddedPlayer/{$type}={$code}/size={$size}/bgcol={$bg_color}/linkcol={$link_color}//" type="text/html" width="{$width}" height="{$height}">
</object>
</object>
BC;

    return $output;
}

/**
 * Returns a link to view the original media at the provider's site.
 *  @param $code
 *    The unique identifier for the third party media.
 *  @param $data
 *    The original data array collected for the media.
 *  @return
 *    A string containing the URL to view the original media.
 */
function emaudio_bandcamp_embedded_link($code, $data = array()) {
    return $data['embed']; //have another look at this
}

/**
 *  @TODO
 *  Not sure why this is there -- provided for already w/ hook_emfield_subtheme.
 */
function EMMODULE_PROVIDER_subtheme() {

}

// @TODO this hook is missing from the hooks.php but is used by other providers
function emaudio_bandcamp_audio($embed, $width, $height, $field, $item, $node, $autoplay) {
dsm($field);
dsm($node);
    $item['options'] = emaudio_bandcamp_set_options($item, $width, $height, 'full');

    return theme('emaudio_bandcamp_flash', $item);
}

/**
 *  Returns any theme functions required for this provider.
 */
function emaudio_bandcamp_emfield_subtheme() {
    return array(
        'emaudio_bandcamp_flash' => array(
            'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL, 'options' => array()),
            'file' => 'providers/bandcamp.inc',
        //  'path' => drupal_get_path('module', 'media_example'),
        // don't think we really need this bit but come back and try with and without
        ),
    );
}

function emaudio_bandcamp_preview($embed, $width, $height, $field, $item, $node, $autoplay) {
    $item['options'] = emaudio_bandcamp_set_options($item, $width, $height, 'preview');

    return theme('emaudio_bandcamp_flash', $item);
}

// instead of using a separate inc for the theme we'll put in a theme function call here

/**
 *  @TODO
 *  Not sure why this is there -- provided for already w/ hook_emfield_submenu.
 */
function EMMODULE_PROVIDER_submenu() {

}

/**
 *  Returns any menu pages required by this provider.
 */
function EMMODULE_PROVIDER_emfield_submenu() {

}

/**
 *  Generate random media for the devel module.
 *
 *  If the devel module (at http://drupal.org/project/devel) is installed,
 *  it can be used to generate placeholder content, useful for development.
 *
 *  This function should return an array of URLs to parse. One will be selected
 *  randomly from the array.
 */
function EMMODULE_PROVIDER_content_generate() {
    return array(
        'http://www.example.com/video/drupal-song',
        'http://www.example.com/video/about-emfield',
        'http://www.example.com/video/love-drupal',
    );
}

/**
 *  Build a list of provider files that serve the invoking module.
 *
 *  @param $module
 *    The contributed Embedded Media Field module in question, such as
 *    Embedded Video Field or Embedded Google Wave.
 *  @param $provider
 *    If provided, then we expect the single provider file named.
 *    Otherwise, we expect an array of all provider files supported.
 *  @return
 *    A listing of files built with drupal_system_listing().
 */
function hook_emfield_providers($module, $provider = NULL) {
    return drupal_system_listing("$provider\.inc$", drupal_get_path('module', 'media_example') . "/providers/$module", 'name', 0);
}

/**
 *  Generally used for metadata, this will add an array of database column
 *  information to be saved when present in the field. Note that this hook
 *  is used to return columns to be saved for all Embedded Media Fields,
 *  and is currently unused by the core Embedded Media Field modules.
 */
function hook_emfield_field_columns_extra($field) {
    return array(
        'author' => array(
            'description' => 'The original author of the third party media.',
            'type' => 'varchar',
            'length' => 255,
            'not null' => TRUE,
            'default' => '',
        ),
    );
}

/**
 *  This passes on the original implementation of CCK's hook_widget_settings.
 */
function hook_emfield_widget_settings_extra($op, $widget) {

}

/**
 *  This passes on the original implementation of CCK's hook_widget.
 */
function hook_emfield_widget_extra($form, $form_state, $field, $items, $delta, $module) {

}

/**
 *  Alter the data array of a media item before it is stored in the database.
 */
function hook_emfield_data_alter(&$data, $module, $delta = 0, $node = NULL, $field = NULL, $items = array()) {
//    dsm($data);
//    dsm($module);
//    dsm($delta);
//    dsm($node);
//    dsm($field);
//    dsm($items);
}

/*
 * @param $item
 * nested array of player parameters, options and data
 *
 * @param $width
 * The default width set in the emaudio providers default CCK configuration
 *
 * @param $height
 * The default height set in the emaudio providers default CCK configuration
 *
 * @param $display_type
 * A string literal - either 'full' or 'preview' for the type of display being handled
 *
 *
 */

function emaudio_bandcamp_set_options($item, $width, $height, $display_type = 'full') {

    $options = array();
    $type = $item['data']['type'];
    $size = variable_get('emaudio_bandcamp_' . $type . '_' . $display_type . '_dimensions', EMAUDIO_BANDCAMP_SIZE_DEFAULT);

    if (variable_get('emfield_bandcamp_override_default_dimensions', 0)) {

        switch ($size) {

            case EMAUDIO_BANDCAMP_SIZE_VENTI:
                $width = EMAUDIO_BANDCAMP_SIZE_VENTI_WIDTH;
                $height = EMAUDIO_BANDCAMP_SIZE_VENTI_HEIGHT;
                break;

            case EMAUDIO_BANDCAMP_SIZE_GRANDE:
                $width = EMAUDIO_BANDCAMP_SIZE_GRANDE_WIDTH;
                $height = EMAUDIO_BANDCAMP_SIZE_GRANDE_HEIGHT;
                break;

            case EMAUDIO_BANDCAMP_SIZE_GRANDE2_TRACK:
                $width = EMAUDIO_BANDCAMP_SIZE_GRANDE2_WIDTH;
                $height = EMAUDIO_BANDCAMP_SIZE_GRANDE2_HEIGHT;
                break;

            case EMAUDIO_BANDCAMP_SIZE_GRANDE3_TRACK_ART:
                $width = EMAUDIO_BANDCAMP_SIZE_GRANDE3_WIDTH;
                $height = EMAUDIO_BANDCAMP_SIZE_GRANDE3_HEIGHT;
                break;

            case EMAUDIO_BANDCAMP_SIZE_TALL:
                $width = EMAUDIO_BANDCAMP_SIZE_TALL_WIDTH;
                $height = EMAUDIO_BANDCAMP_SIZE_TALL_HEIGHT;
                break;

            case EMAUDIO_BANDCAMP_SIZE_TALL2_TRACK:
                $width = EMAUDIO_BANDCAMP_SIZE_TALL2_WIDTH;
                $height = EMAUDIO_BANDCAMP_SIZE_TALL2_HEIGHT;
                break;

            case EMAUDIO_BANDCAMP_SIZE_SHORT:
                $width = EMAUDIO_BANDCAMP_SIZE_SHORT_WIDTH;
                $height = EMAUDIO_BANDCAMP_SIZE_SHORT_HEIGHT;
                break;
        }
    }

    $options['size'] = $size;
    $options['width'] = $width;
    $options['height'] = $height;
    $options['bg_color'] = variable_get('emaudio_bandcamp_background_color', EMAUDIO_BANDCAMP_BACKGROUND_COLOR_DEFAULT);
    $options['link_color'] = variable_get('emaudio_bandcamp_link_color', EMAUDIO_BANDCAMP_LINK_COLOR_DEFAULT);

    return $options;
}