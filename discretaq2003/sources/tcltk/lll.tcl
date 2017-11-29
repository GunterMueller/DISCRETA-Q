# Michael Braun
# 2002.06.03

wm title . "LLL - solver"

global filename lambda factor beta p
set filename ""
set lambda ""
set factor "100"
set beta "80"
set p "18"

#------------------------
#      parameters
#------------------------

frame .parameters  -bd 2

label .parameters.label1 -text "Parameters"

foreach i {p1 p2 p3 p4 p5 } {
  frame .parameters.$i
  entry .parameters.$i.entry -relief sunken -width 30
  label .parameters.$i.label
  pack .parameters.$i.entry -side right
  pack .parameters.$i.label -side right
}

.parameters.p1.entry config -textvariable filename
.parameters.p1.label config -text File

.parameters.p2.entry config -textvariable lambda
.parameters.p2.label config -text lambda

.parameters.p3.entry config -textvariable factor
.parameters.p3.label config -text c0_factor

.parameters.p4.entry config -textvariable beta
.parameters.p4.label config -text beta

.parameters.p5.entry config -textvariable p
.parameters.p5.label config -text p

pack \
 .parameters.label1 \
 .parameters.p3 \
 .parameters.p4 \
 .parameters.p5 -side top -fill x

pack .parameters

#------------------------
#      buttons
#------------------------

frame .buttons -bd 2
button .buttons.b1 -text "Solve" -width 18 -command "solve"
button .buttons.b2 -text "Cancel" -width 18 -command exit

pack \
 .buttons.b1 \
 .buttons.b2 -side left -fill y

pack .buttons

#------------------------------
#     SOLVE
#------------------------------

proc solve { } {
  global filename lambda factor bkz beta p
  set param "solvedesign "
  append param $factor " " $lambda " 1 bkz " $beta " " $p " " $filename
  set param
  puts $param
  set initfile [open "compute.txt" w+]
  puts $initfile $param
  close $initfile
  exec <@stdin >@stdout execfile compute.txt
  exec <@stdin >@stdout rm compute.txt
  exit
}

#------------------------------
#     DEFAULT SETTINGS
#------------------------------

proc default { } {
  global filename lambda factor beta p
  if [catch {open "solve_info.txt" r} fileId] {
    puts stderr $fileId
  } else {
    set inhalt [open "solve_info.txt" r+]
    set filename [gets $inhalt]
    set lambda [gets $inhalt]
    close $inhalt
  }
}

default



