<?php
/**
 * Expose the signable status of a petition as a context condition.
 */
class petition_context_condition_signable extends context_condition {

  /**
   * {@inheritdoc}
   */
  function condition_values() {
    return array(
      'signable' => t('Petition is signable')
    );
  }

  /**
   * Execute the context condition.
   *
   * @param obj $node
   *   A drupal node object.
   */
  function execute($node) {
    if ($node && $node->type == 'petition') {
      $petition = PetitionsController::loadObject($node->nid);
      if ($petition instanceof PetitionItem) {
        $is_signable = $petition->isSignable();
        foreach ($this->get_contexts() as $context) {
          $values = $this->fetch_from_context($context, 'values');

          // Determine if the context has been met.
          $context_met = (!empty($values['signable']) ? $is_signable : !$is_signable);
          if ($context_met) {
            $this->condition_met($context);
          }
        }
      }
    }
  }
}
