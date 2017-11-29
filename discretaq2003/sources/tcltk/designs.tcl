wm title . "DISCRETA-Q"

puts "-------------------------------------------------------------------------"
puts "Welcome to "
puts " "
puts "  #####   #   ####    ####   #####   ######  #####    ##          ####  "
puts "  #    #  #  #       #    #  #    #  #         #     #  #        #    # "
puts "  #    #  #   ####   #       #    #  #####     #    #    #  ###  #    # "
puts "  #    #  #       #  #       #####   #         #    ######       #  # # "
puts "  #    #  #  #    #  #    #  #   #   #         #    #    #       #   ## "
puts "  #####   #   ####    ####   #    #  ######    #    #    #        ##### "
puts " "
puts " "


#------------------------
#      menubar
#------------------------

menu .menubar
. config -menu .menubar

foreach m {Group Matrix Solve Report} {
  set $m [menu .menubar.m$m]
  .menubar add cascade -label $m -menu .menubar.m$m
}

$Group add command -label "Choose Group" \
  -command exit

$Group add separator

$Group add command -label "Quit program" \
  -command quitprogram

$Matrix add command -label "Compute Matrix" \
  -command computematrix

$Matrix add command -label "Show Matrix" \
  -command exit


$Solve add command -label "Solve (LLL)" \
  -command exit

$Solve add command -label "Solve (Knuth's Dance)" \
  -command exit

$Solve add command -label "Show Solutions" \
  -command exit


$Report add command -label "show report" \
  -command exit

$Report add command -label "make .ps file" \
  -command exit

$Report add command -label "print report" \
  -command exit


#------------------------
#      group
#------------------------
frame .row1 -bd 2

frame .row1.group  -bd 2

label  .row1.group.label -text "Prescribed Group:"

foreach i {p1 p2 p3 p4} {
  frame .row1.group.$i
  entry .row1.group.$i.entry -relief sunken -width 18
  label .row1.group.$i.label
  pack .row1.group.$i.entry -side right
  pack .row1.group.$i.label -side right
}

.row1.group.p1.entry config -textvariable groupstring
.row1.group.p1.label config -text group:

.row1.group.p2.entry config -textvariable n
.row1.group.p2.label config -text n:

.row1.group.p3.entry config -textvariable q
.row1.group.p3.label config -text q:

.row1.group.p4.entry config -textvariable grouporder
.row1.group.p4.label config -text order:


pack \
 .row1.group.label \
 .row1.group.p1 \
 .row1.group.p2 \
 .row1.group.p3 \
 .row1.group.p4  -side top -fill x

#------------------------
#      param
#------------------------

frame .row1.param  -bd 2
label .row1.param.label -text "Design Parameters:"
foreach i {p1 p2 p3} {
  frame .row1.param.$i
  entry .row1.param.$i.entry -relief sunken -width 40
  label .row1.param.$i.label
  pack .row1.param.$i.entry -side right
  pack .row1.param.$i.label -side right
}

.row1.param.p1.entry config -textvariable t
.row1.param.p1.label config -text t:

.row1.param.p2.entry config -textvariable k
.row1.param.p2.label config -text k:

.row1.param.p3.entry config -textvariable lambda
.row1.param.p3.label config -text lambda:


pack \
 .row1.param.label \
 .row1.param.p1 \
 .row1.param.p2 \
 .row1.param.p3  -side top -fill x


pack .row1.group .row1.param -side top -fill x

pack .row1 -side top -fill x

#------------------------------
#     Computematrix
#------------------------------

proc computematrix { } {
  global groupstring n q t k
  set param "compute_kmm -q "
  append param $q " -group " $groupstring " " $n " -t " $t " -k "  $k
  set param
  puts $param
  set initfile [open "compute.txt" w+]
  puts $initfile $param
  close $initfile
  exec <@stdin >@stdout execfile compute.txt
  exec <@stdin >@stdout rm compute.txt
}

#------------------------------
#     EXIT
#------------------------------

proc quitprogram { } {
  puts "Good bye!"
  puts "-------------------------------------------------------------------------"
  exit
}
