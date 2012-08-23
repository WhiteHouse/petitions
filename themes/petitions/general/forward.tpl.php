<?php
if (arg(0) == 'petition') {
print variable_get('wh_petitions_email_forward_text', 'Dear Friends,

I wanted to let you know about an official petition I have signed at WhiteHouse.gov. Will you add your name to mine?  If this petition gets !signatures_needed signatures by !date_needed, the White House will review it and respond!

You can view and sign the petition here: !shorturl

Here\'s some more information about this petition:
!petition_description');
}
else {
print variable_get('wh_petitions_email_forward_response_text', 'Dear Friends,

I wanted to let you know about an official petition I have signed at WhiteHouse.gov. Will you add your name to mine?  If this petition gets !signatures_needed signatures by !date_needed, the White House will review it and respond!

You can view and sign the petition here: !shorturl

Here\'s some more information about this petition:
!petition_description');
}