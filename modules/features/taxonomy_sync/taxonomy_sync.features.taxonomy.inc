<?php

/**
 * Implementation of hook_taxonomy_default_vocabularies().
 */
function taxonomy_sync_taxonomy_default_vocabularies() {
  return array(
    'issues' => array(
      'name' => 'Issues',
      'machine_name' => 'issues',
      'description' => 'Contains Issues from the main site.',
      'hierarchy' => '1',
      'module' => 'taxonomy',
      'weight' => '0',
      'rdf_mapping' => array(
        'rdftype' => array(
          '0' => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            '0' => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            '0' => 'rdfs:comment',
          ),
        ),
      ),
    ),
  );
}
