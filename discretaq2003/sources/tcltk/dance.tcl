wm title . "Knuth's Dance - solver for lambda=1"
global filename
set filename "IncMat.txt"
#------------------------
#      parameters
#------------------------

frame .parameters  -bd 2

foreach i {p1} {
  frame .parameters.$i
  entry .parameters.$i.entry -relief sunken -width 35
  label .parameters.$i.label
  pack .parameters.$i.entry -side right
  pack .parameters.$i.label -side right
}

.parameters.p1.entry config -textvariable filename
.parameters.p1.label config -text matrix:


pack \
 .parameters.p1 -side top -fill x

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
  global filename lambda factor bound bkz beta p file
  set param "stripcolumnsdance 1 "
  append param $filename " | dance"
  set param
  set initfile [open "compute.txt" w+]
  puts $initfile $param
  close $initfile
  exec <@stdin >@stdout execfile compute.txt
  exec <@stdin >@stdout rm compute.txt
}

#------------------------------
#     DEFAULT SETTINGS
#------------------------------

proc default { } {
  global filename 
  if [catch {open "solve_info.txt" r} fileId] {
    puts stderr $fileId
  } else {
    set inhalt [open "solve_info.txt" r+]
    set filename [gets $inhalt]
    close $inhalt
  }
}
proc default { } {
  global filename
  set filename "IncMat.txt"
}

