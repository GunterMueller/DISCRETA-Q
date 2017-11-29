wm title . "Choose prescribed group"

global textvariable selection
set selection "singer"

global textvariable pow
set pow "0"

global textvariable h2farbe
set h2farbe "white"

frame .row1 -bd 2 -relief ridge 
frame .row2 -bd 2 -relief ridge 
frame .row3 -bd 2 -relief ridge 
frame .row4 -bd 2 -relief ridge 

foreach i {p1 p2} {
  frame .row1.$i
  entry .row1.$i.entry -relief sunken -width 30 
  label .row1.$i.label -anchor w
  pack .row1.$i.entry -side right
  pack .row1.$i.label -side right
}
.row1.p1.entry config -textvariable n
.row1.p1.label config -text n
.row1.p2.entry config -textvariable q
.row1.p2.label config -text q
pack .row1.p1 .row1.p2 -side top -fill x


frame .row3.p -bd 2 

checkbutton .row3.p.g9 -text "power" -variable pow  -anchor w 

foreach i {p1} {
  frame .row3.p.$i  
  entry .row3.p.$i.entry -relief sunken -width 24
  label .row3.p.$i.label -anchor w
  pack .row3.p.$i.entry -side right
  pack .row3.p.$i.label -side right
}
.row3.p.p1.entry config -textvariable power
.row3.p.p1.label config -text ""

pack .row3.p.g9 .row3.p.p1 -side left -fill x

label .row3.label1 -text "Cyclic Groups"
radiobutton .row2.g1 -text "One group" -variable selection -value one -anchor w
radiobutton .row2.g2 -text "General linear group" -variable selection -value gl -anchor w
radiobutton .row2.g3 -text "Complete monomial group" -variable selection -value mono -anchor w
radiobutton .row2.g4 -text "Symmetric group" -variable selection -value sym -anchor w
radiobutton .row2.g5 -text "Borel group" -variable selection -value borel -anchor w
radiobutton .row2.g6 -text "Normalizer of Singer cycle" -variable selection -value nsinger -anchor w
radiobutton .row3.g8 -text "Frobenius automorphism" -variable selection -value frob -anchor w
radiobutton .row3.g7 -text "Singer cycle" -variable selection -value singer -anchor w

pack .row2.g1 \
.row2.g2 \
.row2.g3 \
.row2.g4 \
.row2.g5 \
.row2.g6 \
-side top -fill x


pack .row3.label1 \
.row3.g8 \
.row3.g7 \
.row3.p \
-side top -fill x



button .row4.b1 -text "Cancel" -width 13 -command "exit"
button .row4.b2 -text "Choose group" -width 13 -command "choose" 


pack .row4.b2 .row4.b1  -side left -expand 1


pack .row1 .row2 .row3 .row4 -side top -fill x


proc choose { } {
  global selection n q pow power
  set param "setgroup"
  append param " -q " $q
  append param " -n " $n
  append param " -group "
  if {$selection == "one"} {
    append param "one"
  }
  if {$selection == "gl"} {
    append param "general_linear"
  }
  if {$selection == "mono"} {
    append param "monomial"
  }
  if {$selection == "sym"} {
    append param "symmetric"
  }
  if {$selection == "borel"} {
    append param "borel"
  }
  if {$selection == "nsinger"} {
    append param "normalizer_singer"
  }
  if {$selection == "frob"} {
    append param "frobenius"
  }
  if {$selection == "singer"} {
    append param "singer"
  }
  if {$pow == "1"} {
    append param " power "
    append param $power

  }
  puts $param
  set initfile [open "compute.txt" w+]
  puts $initfile $param
  close $initfile 
  exec <@stdin >@stdout execfile compute.txt
  exec <@stdin >@stdout rm compute.txt
  exit;
}


