<?php
  $cspheader = "Content-Security-Policy:".
               "default-src 'self';".
               "script-src 'self' 'nonce-111-111-111' 'nonce-222-222-222' *.example70.com *.example60.com".
               "";
  header($cspheader);
?>

<?php include 'index.html';?>

