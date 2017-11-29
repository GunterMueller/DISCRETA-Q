wm title . "Choose prescribed automorphism group"

#------------------------
#     row1
#------------------------

frame .row1 -bd 2

#------------------------
#     row1.col1
#------------------------

frame .row1.col1 -bd 2

label .row1.col1.label1 -text "trivial groups:"

button .row1.col1.b1 -text "one {id}" -width 25 \
 -command "One void"

button .row1.col1.b2 -text "general linear GL(n,q)" -width 25 \
 -command "Gl void"

label .row1.col1.label2 -text "well known groups:"

button .row1.col1.b3 -text "Center of GL" -width 25 \
 -command "Center void"

button .row1.col1.b4 -text "borel B(n,q)" -width 25 \
 -command "Borel void"

button .row1.col1.b5 -text "singer cycle SC(n,q)" -width 25 \
 -command "Singer void"

button .row1.col1.b6 -text "frobenius automorphism F(n,q)" -width 25 \
 -command "Frobenius void"

button .row1.col1.b7 -text "normalizer of singer N_SC(n,q)" -width 25 \
 -command "N_singer void"

pack .row1.col1.label1 \
 .row1.col1.b1 \
 .row1.col1.b2 \
 .row1.col1.label2 \
 .row1.col1.b4 \
 .row1.col1.b5 \
 .row1.col1.b6 \
 .row1.col1.b7  -side top -fill x 

#------------------------
#      row1.col2
#------------------------

frame .row1.col2 -bd 2

label .row1.col2.label1 -text "wreath products:"

button .row1.col2.b1 -text "monomial M(n,q)" -width 25 \
 -command "Mono void"

button .row1.col2.b2 -text "diagonal" -width 25 \
 -command "Diag void"

button .row1.col2.b3 -text "symmetric S_n" -width 25 \
 -command "Perm void"

button .row1.col2.b4 -text "cyclic C_n" -width 25 \
 -command "Cperm void"


pack .row1.col2.label1 \
 .row1.col2.b1 \
 .row1.col2.b2 \
 .row1.col2.b3 \
 .row1.col2.b4 -side top -fill x

#------------------------
#      row1.col2
#------------------------

frame .row1.col3 -bd 2

label .row1.col3.label1 -text "binary operation:"

button .row1.col3.b1 -text "x (direct product)" -width 25 \
 -command "Dp void"

button .row1.col3.b2 -text "wr (wreath product)" -width 25 \
 -command ""

label .row1.col3.label2 -text "unary operation:"
button .row1.col3.b3 -text "add fixed 1-subspace" -width 25 \
 -command "Fix void"


label .row1.col3.label3 -text "by file:"
button .row1.col3.b4 -text "choose file" -width 25 \
 -command "Byfile void"

foreach i {p1} {
  frame .row1.col3.$i
  entry .row1.col3.$i.entry -relief sunken -width 24
  label .row1.col3.$i.label 
  pack .row1.col3.$i.entry -side right
  pack .row1.col3.$i.label -side right
}

.row1.col3.p1.entry config -textvariable file
.row1.col3.p1.label config -text file:

pack .row1.col3.label1 \
 .row1.col3.b1 \
-side top -fill x

pack .row1.col1 \
 .row1.col2 \
 .row1.col3 -side left -fill y

#------------------------
#      row2
#------------------------

frame .row2 -bd 2
button .row2.b1 -text "Clear all items" -width 40 -command Clear
button .row2.b2 -text "Choose group" -width 40 -command "Choose; exit"


pack .row2.b1 \
 .row2.b2  -side left -fill y

#------------------------
#      row3
#------------------------

frame .row3 -bd 2

foreach i {p2 p3} {
  frame .row3.$i
  entry .row3.$i.entry -relief sunken -width 39
  label .row3.$i.label 
  pack .row3.$i.entry -side right
  pack .row3.$i.label -side right
}

.row3.p2.entry config -textvariable q
.row3.p2.label config -text q

.row3.p3.entry config -textvariable m
.row3.p3.label config -text n

pack .row3.p3 \
 .row3.p2 -side left -fill x

#------------------------ 
#      row4 
#------------------------ 
 
frame .row4 -bd 2 
 
foreach i {p1} { 
  frame .row4.$i 
  entry .row4.$i.entry -relief sunken -width 70 
  label .row4.$i.label  
  pack .row4.$i.entry -side right 
  pack .row4.$i.label -side right 
} 
 
.row4.p1.entry config -textvariable group 
.row4.p1.label config -text group: 
 
pack .row4.p1 -side left -fill x 


#------------------------
#      pack
#------------------------

pack .row3 .row4 .row1 .row2 -side top -fill x

#------------------------------
#      One
#------------------------------

proc One void {
  global group m
  set prevgroup $group 
  set name " one "
  set group [concat $prevgroup$name$m]
}

#------------------------------
#      GL
#------------------------------

proc Gl void {
  global group m
  set prevgroup $group
  set name " generallinear "
  set group [concat $prevgroup$name$m]
}

#------------------------------
#      Center
#------------------------------

proc Center void {
  global group m
  set prevgroup $group 
  set name " Center "
  set group [concat $prevgroup$name$m]
}

#------------------------------
#      Diag
#------------------------------

proc Diag void {
  global group m
  set prevgroup $group 
  set name " diagonal "
  set group [concat $prevgroup$name$m]
}

#------------------------------
#      Mono
#------------------------------

proc Mono void {
  global group m
  set prevgroup $group 
  set name " monomial "
  set group [concat $prevgroup$name$m]
}

#------------------------------
#      Perm
#------------------------------

proc Perm void {
  global group m
  set prevgroup $group 
  set name " symmetric "
  set group [concat $prevgroup$name$m]
}

#------------------------------
#      Cperm
#------------------------------

proc Cperm void {
  global group m
  set prevgroup $group 
  set name " cyclicperm "
  set group [concat $prevgroup$name$m]
}

#------------------------------
#      Dihedral
#------------------------------

proc Dihedral void {
  global group m
  set prevgroup $group 
  set name " Dihedral "
  set group [concat $prevgroup$name$m]
}

#------------------------------
#      Borel
#------------------------------

proc Borel void {
  global group m
  set prevgroup $group 
  set name " borel "
  set group [concat $prevgroup$name$m]
}

#------------------------------
#     Singer
#------------------------------

proc Singer void {
  global group m
  set prevgroup $group 
  set name " singer "
  set group [concat $prevgroup$name$m]
}

#------------------------------
#     Frobenius
#------------------------------

proc Frobenius void {
  global group m
  set prevgroup $group 
  set name " frobenius "
  set group [concat $prevgroup$name$m]
}

#------------------------------
#     Normalizer singer
#------------------------------

proc N_singer void {
  global group m
  set prevgroup $group
  set name " normalizersinger "
  set group [concat $prevgroup$name$m]
}

#------------------------------
#      add fixed 1-subspace
#------------------------------

proc Fix void {
  global group m
  set prevgroup $group 
  set name " one 1 x"
  set group [concat $prevgroup$name]
}

#------------------------------
#      x
#------------------------------

proc Dp void {
  global group m
  set prevgroup $group 
  set name " x"
  set group [concat $prevgroup$name]
}

#------------------------------
#      By File
#------------------------------

proc Byfile void {
  global group file
  append group " file ../../gen/" $file
  set group
}

#------------------------------
#      clear all items
#------------------------------

proc Clear { }  {
  global group m q
  set group ""
  set m ""
  set q ""
}

#------------------------------
#     Choose
#------------------------------

proc Choose { } {
  global group q 
  set param " discretaq -setgroup -q "
  append param $q " -group " $group
  set param
  puts setgroup
  puts $param
  set initfile [open "compute.txt" w+]
  puts $initfile $param
  close $initfile 
  exec <@stdin >@stdout execfile compute.txt
  exec <@stdin >@stdout rm compute.txt
}


