wm title . "DISCRETA-Q"

puts "-------------------------------------------------------------------------"
puts "Welcome to DISCRETAQ"
puts " "


global n q t k lambda groupname groupstring grouporder
set n "7"
set q "2"
set t "2"
set k "3"
set groupname "normalizersinger7"
set groupstring "normalizersinger 7"
set grouporder "889"
set lambda "7"


global textvariable verbose
set verbose "1"

global textvariable shownfile
set shownfile "../../sources/help/discreta_q.txt"

global textvariable solve
set solve "lll"


frame .mbar -relief raised -bd 2
pack .mbar -side top -fill x

menubutton .mbar.group -text "Group" -underline 0 -menu .mbar.group.menu
menubutton .mbar.matrix -text "Matrix" -underline 0 -menu .mbar.matrix.menu
menubutton .mbar.designs  -text "Designs" -underline 0 -menu .mbar.designs.menu
menubutton .mbar.report -text "Report" -underline 0 -menu .mbar.report.menu
menubutton .mbar.help -text "Help" -underline 0 -menu .mbar.help.menu

pack  .mbar.group .mbar.matrix .mbar.designs  -side left



menu .mbar.group.menu
.mbar.group.menu add command -label "Choose" -underline 0 -command "choose"
.mbar.group.menu add separator
.mbar.group.menu add command -label "Quit" -underline 0 -command "quitprogram"


menu .mbar.matrix.menu
.mbar.matrix.menu add command -label "Compute M^G_{t,k}" -underline 0 -command "computematrix; showmatrix"
.mbar.matrix.menu add separator
.mbar.matrix.menu add command -label "Show M^G_{t,k}" -underline 0 -command "showmatrix"
.mbar.matrix.menu add command -label "Show G-orbits on k-subspaces" -underline 0 -command "showorbits"


menu .mbar.designs.menu
.mbar.designs.menu add command -label "Solve" -underline 0 -command "solve; showsolutions"
.mbar.designs.menu add radiobutton -label "LLL" -variable solve -value lll
.mbar.designs.menu add radiobutton -label "Dance (lambda = 1)" -variable solve -value dance
.mbar.designs.menu add separator
.mbar.designs.menu add command -label "Show solutions" -underline 0 -command "showsolutions"

menu .mbar.report.menu
.mbar.report.menu add checkbutton -label "design theory" -variable  cbtheory
.mbar.report.menu add checkbutton -label "with group" -variable cbgroup
.mbar.report.menu add checkbutton -label "with matrix" -variable  cbmatrix
.mbar.report.menu add checkbutton -label "with orbit representatives" -variable  cbreps
.mbar.report.menu add checkbutton -label "with solutions" -variable cbsolutions
.mbar.report.menu add command -label "Report (.pdf)" -underline 0 -command "exit"

menu .mbar.help.menu
.mbar.help.menu add command -label "About" -underline 0 -command "showabout"
.mbar.help.menu add command -label "Show manual " -underline 0 -command "exit"


tk_menuBar .mbar .mbar.file .mbar.group .mbar.matrix .mbar.designs .mbar.report .mbar.help

focus .mbar

frame .z -bd 2

frame .z.row1 -bd 2
frame .z.row2 -bd 2


pack .z.row1 .z.row2 -side left -fill y

frame .z.row1.col1 -relief ridge -bd 2
label  .z.row1.col1.label -text "Vector Space"
foreach i {p1 p2} {
  frame .z.row1.col1.$i
  entry .z.row1.col1.$i.entry -relief sunken -width 30
  label .z.row1.col1.$i.label
  pack .z.row1.col1.$i.entry -side right
  pack .z.row1.col1.$i.label -side right
}
.z.row1.col1.p1.entry config -textvariable n
.z.row1.col1.p1.label config -text n
.z.row1.col1.p2.entry config -textvariable q
.z.row1.col1.p2.label config -text q
pack .z.row1.col1.label .z.row1.col1.p1 .z.row1.col1.p2 -side top -fill x

frame .z.row1.col2 -relief ridge -bd 2
label  .z.row1.col2.label -text "Prescibed Group G"
foreach i {p1 p2} {
  frame .z.row1.col2.$i
  entry .z.row1.col2.$i.entry -relief sunken -width 30
  label .z.row1.col2.$i.label
  pack .z.row1.col2.$i.entry -side right
  pack .z.row1.col2.$i.label -side right
}
.z.row1.col2.p1.entry config -textvariable groupname
.z.row1.col2.p1.label config -text Group
.z.row1.col2.p2.entry config -textvariable grouporder
.z.row1.col2.p2.label config -text Order
pack .z.row1.col2.label .z.row1.col2.p1 .z.row1.col2.p2 -side top -fill x

pack .z.row1.col1 .z.row1.col2 -side top -fill x

frame .z.row2.col1 -relief ridge -bd 2
label  .z.row2.col1.label -text "Incidence Matrix M^G_{t,k}"
foreach i {p1 p2 p3} {
  frame .z.row2.col1.$i
  entry .z.row2.col1.$i.entry -relief sunken -width 30
  label .z.row2.col1.$i.label
  pack .z.row2.col1.$i.entry -side right
  pack .z.row2.col1.$i.label -side right
}
.z.row2.col1.p1.entry config -textvariable t
.z.row2.col1.p1.label config -text t
.z.row2.col1.p2.entry config -textvariable k
.z.row2.col1.p2.label config -text k
.z.row2.col1.p3.entry config -textvariable matrixfile
.z.row2.col1.p3.label config -text File
pack .z.row2.col1.label .z.row2.col1.p1 .z.row2.col1.p2  -side top -fill x


frame .z.row2.col2 -relief ridge -bd 2
label  .z.row2.col2.label -text "Design Parameter"
foreach i {p1} {
  frame .z.row2.col2.$i
  entry .z.row2.col2.$i.entry -relief sunken -width 30
  label .z.row2.col2.$i.label
  pack .z.row2.col2.$i.entry -side right
  pack .z.row2.col2.$i.label -side right
}
.z.row2.col2.p1.entry config -textvariable lambda
.z.row2.col2.p1.label config -text lambda
pack .z.row2.col2.label .z.row2.col2.p1  -side top -fill x

pack .z.row2.col1 .z.row2.col2 -side top -fill x

frame .s
frame .t
eval {text .t.text -wrap none \
  -xscrollcommand [list .s.xscroll set] \
  -yscrollcommand [list .t.yscroll set]} -width 74 -height 16
scrollbar .s.xscroll -orient horizontal\
  -command [list .t.text xview]
scrollbar .t.yscroll -orient vertical \
  -command [list .t.text yview]
pack .t.text .t.yscroll  -side left -fill y

pack .s.xscroll -side top  -fill x

frame .r 
entry .r.entry -relief sunken -width 73
label .r.label
pack .r.label .r.entry -side left



.r.entry config -textvariable shownfile
.r.label config -text File

pack .z .r .t .s  -side top  -fill x


proc showfile { } {
  exec <@stdin >@stdout wish ../../sources/tcltk/show_file.tcl
}


proc choose { } {
  global groupstring groupname n q grouporder
  exec <@stdin >@stdout wish ../../sources/tcltk/choose.tcl
  if [catch {open "group_info.txt" r} fileId] {
    puts stderr $fileId
  } else {
    set inhalt [open "group_info.txt" r+]
    set groupstring [gets $inhalt]
    set groupname [gets $inhalt]
    set n [gets $inhalt]
    set q [gets $inhalt]
    set grouporder [gets $inhalt]
    close $inhalt
  }
}

proc computematrix { } {
  global q t k groupstring verbose
  set param "discretaq -kramermesnermatrix"
  append param " -q " $q
  append param " -t " $t
  append param " -k " $k
  append param " -group " $groupstring
  if $verbose==1 {
    append param " -v "
  }
  puts $param
  set initfile [open "compute.txt" w+]
  puts $initfile $param
  close $initfile
  exec <@stdin >@stdout execfile compute.txt
  exec <@stdin >@stdout rm compute.txt
}

proc showmatrix { } {
  global matrixfile n q t k groupname
  set matrixfile "A_inf"
  append matrixfile "_" $groupname
  append matrixfile "_dual_n" $n
  append matrixfile "_q" $q
  append matrixfile "_t" $t
  append matrixfile "_k" $k
  append matrixfile ".txt"
  if [catch {open $matrixfile r} fileId] {
    puts stderr $fileId
  } else {
    showdatei $matrixfile
  }
}

proc showdatei datei {
  global shownfile
  set shownfile $datei
  .t.text delete 1.0 end
  set d [open $datei]
  while {![eof $d]} {
    .t.text insert end [read $d 1000]
  }
  close $d
}
showdatei $shownfile


proc solve { } {
  global solve matrixfile lambda groupname q n t k
  if {$lambda == ""} {
    puts "ERROR: what is lambda?"
  } else {
  set matrixfile "A_inf"
  append matrixfile "_" $groupname
  append matrixfile "_dual_n" $n
  append matrixfile "_q" $q
  append matrixfile "_t" $t
  append matrixfile "_k" $k
  append matrixfile ".txt"
  if [catch {open $matrixfile r} fileId] {
    puts stderr $fileId
  } else {
    if {$solve == "lll"} {
      set initfile [open "solve_info.txt" w+]
      puts $initfile $matrixfile
      puts $initfile $lambda
      close $initfile
      exec <@stdin >@stdout wish ../../sources/tcltk/lll.tcl
      exec <@stdin >@stdout rm solve_info.txt
    } else {
      set lambda 1
      set param "stripcolumnsdance 1 " 
      append param $matrixfile
      append param " | dance" 
      puts $param
      set initfile [open "compute.txt" w+]
      puts $initfile $param
      close $initfile
      exec <@stdin >@stdout execfile compute.txt
      exec <@stdin >@stdout rm compute.txt
    } 
    if [catch {open solutions r} fileId] {
      puts stderr $fileId
    } else {
      set param "cp solutions Solutions_lambda"
      append param $lambda
      append param "_" $groupname
      append param "_dual_n" $n
      append param "_q" $q
      append param "_t" $t
      append param "_k" $k
      append param ".txt"
      puts $param  
      set initfile [open "compute.txt" w+]
      puts $initfile $param
      close $initfile
      exec <@stdin >@stdout execfile compute.txt
      exec <@stdin >@stdout rm compute.txt solutions
    }
  }
  }
}

proc showsolutions { } {
  global lambda groupname q n t k
  set param "Solutions_lambda"
  append param $lambda
  append param "_" $groupname
  append param "_dual_n" $n
  append param "_q" $q
  append param "_t" $t
  append param "_k" $k
  append param ".txt"
  if [catch {open $param r} fileId] {
    puts stderr $fileId
  } else {
    showdatei $param
  }
}

proc showorbits { } {
  global groupname q n k
  set param "Orbits"
  append param "_" $groupname
  append param "_dual_n" $n
  append param "_q" $q
  append param "_k" $k
  append param ".txt"
  if [catch {open $param r} fileId] {
    puts stderr $fileId
  } else {
    showdatei $param
  }
}


proc showabout { } {
 showdatei "../../sources/help/discreta_q.txt"
}




proc quitprogram { } {
  puts "Good bye!"
  exit
}







