description "Unit tests for the Zen Grids system."

stylesheet 'sass/function-zen-direction-flip.scss',  :media => 'all', :to => 'function-zen-direction-flip.scss'
stylesheet 'sass/function-zen-grid-item-width.scss', :media => 'all', :to => 'function-zen-grid-item-width.scss'
stylesheet 'sass/function-zen-half-gutter.scss',     :media => 'all', :to => 'function-zen-half-gutter.scss'
stylesheet 'sass/function-zen-unit-width.scss',      :media => 'all', :to => 'function-zen-unit-width.scss'
stylesheet 'sass/zen-clear.scss',                    :media => 'all', :to => 'zen-clear.scss'
stylesheet 'sass/zen-grid-container.scss',           :media => 'all', :to => 'zen-grid-container.scss'
stylesheet 'sass/zen-grid-flow-item.scss',           :media => 'all', :to => 'zen-grid-flow-item.scss'
stylesheet 'sass/zen-grid-item-base.scss',           :media => 'all', :to => 'zen-grid-item-base.scss'
stylesheet 'sass/zen-grid-item.scss',                :media => 'all', :to => 'zen-grid-item.scss'
stylesheet 'sass/zen-nested-container.scss',         :media => 'all', :to => 'zen-nested-container.scss'

file 'test-results/function-zen-direction-flip.css'
file 'test-results/function-zen-grid-item-width.css'
file 'test-results/function-zen-half-gutter.css'
file 'test-results/function-zen-unit-width.css'
file 'test-results/zen-clear.css'
file 'test-results/zen-grid-container.css'
file 'test-results/zen-grid-flow-item.css'
file 'test-results/zen-grid-item-base.css'
file 'test-results/zen-grid-item.css'
file 'test-results/zen-nested-container.css'

file 'README.txt'

help %Q{
To run the unit tests, simply run "compass compile" and compare the generated
CSS to those in the results directory.
  diff -r results css
}

welcome_message %Q{
You rock! The unit tests for the Zen Grids system are now installed.
}
