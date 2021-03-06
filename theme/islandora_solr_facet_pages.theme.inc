<?php

/**
 * @file
 * Theme functions.
 *
 * Contains all theme related functionality. Theme and preprocess
 * functions.
 */

use Drupal\Core\Url;

/**
 * Implements template_preprocess_HOOK().
 */
function template_preprocess_islandora_solr_facet_pages_results(&$variables) {
  // Add template suggestion based on path name.
  $variables['template_files'][] = 'islandora-solr-facet-pages-results--' . $variables['path'];
}

/**
 * Implements template_preprocess_HOOK().
 */
function template_preprocess_islandora_solr_facet_pages_wrapper(&$variables) {
  $total = $variables['pager_data']['total'];
  $from = $variables['pager_data']['offset'] + 1;
  $to = $variables['pager_data']['offset'] + $variables['pager_data']['limit'];
  if ($to > $total) {
    $to = $total;
  }

  if ($total > 0) {
    $results_count = t('Showing @from - @to of @total', [
      '@from' => $from,
      '@to' => $to,
      '@total' => $total,
    ]);
  }
  else {
    $results_count = t('No results found.');
  }

  $variables['results_count'] = $results_count;

  // Add template suggestion based on path name.
  $variables['template_files'][] = 'islandora-solr-facet-pages-wrapper--' . $variables['path'];
}

/**
 * Theme function to render the alphabetical pager.
 *
 * @param array $vars
 *   Array of parameters, including:
 *   - facet_queries: Facet queries returned from Solr.
 *     eg: solr_field:A* => 1234
 *     This array is used to check the bucket size. An empty bucket won't be
 *     rendered into a link.
 *   - fq_map: Array which maps query filter so prefixes.
 *     eg: solr_field:A* => A
 *     Used to render the link values.
 *   - search_prefix: Currently active prefix for this page. Used to set active
 *     classes.
 *   - path: Currently active path which is set in the admin interface to
 *     decide what Solr field to facet on. Used to create URL's.
 *     eg: url('browse/' . $path).
 *
 * @return string
 *   Return rendered alphabetical pager
 *
 * @see islandora_solr_facet_pages_theme()
 * @see islandora_solr_facet_pages_build_letterer()
 */
function theme_islandora_solr_facet_pages_letterer(array &$vars) {
  $prefix = $vars['search_prefix'];
  $facet_queries = $vars['facet_queries'];
  $fq_map = $vars['fq_map'];
  $path = $vars['path'];

  $output = '<ul class="islandora-solr-facet-pages-letterer">';

  $output .= '<li class="letter first' . ((t('ALL') == $prefix) ? ' active' : '') . '">';
  $output .= '<a href="' . Url::fromRoute('islandora_solr_facet_pages.callback', ['path' => $path])->toString() . '" title="' . t('Browse all') . '">' . t('ALL') . '</a>';

  $output .= '</li>';

  // Loop over facet queries to render letters.
  foreach ($facet_queries as $query => $count) {
    $value = $fq_map[$query];

    $output .= '<li class="letter' . (($value == $prefix) ? ' active' : '') . '">';

    // Create link if facet bucket is not empty.
    if ($count > 0) {
      $url = Url::fromRoute('islandora_solr_facet_pages.callback', ['path' => $path, 'prefix' => $value])->toString();
      $output .= '<a href="' . $url . '" title="' . t('Browse starting with @letter', ['@letter' => $value]) . '">' . $value . '</a>';
    }
    else {
      $output .= $value;
    }
    $output .= '</li>';
  }
  $output .= '</ul>';
  return $output;
}
