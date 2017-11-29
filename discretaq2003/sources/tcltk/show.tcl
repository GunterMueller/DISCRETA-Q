wm title . "Show"

#------------------------------
#      Scrolled_Text
#------------------------------

proc Scrolled_Text { f args } {
  frame $f
  eval {text $f.text -wrap none \
    -xscrollcommand [list $f.xscroll set] \
    -yscrollcommand [list $f.yscroll set]} $args 
  scrollbar $f.xscroll -orient horizontal \
    -command [list $f.text xview]
  scrollbar $f.yscroll -orient vertical \
    -command [list $f.text yview]
  grid $f.text $f.yscroll -sticky news
  grid $f.xscroll -sticky news
  grid rowconfigure $f 0 -weight 1
  grid columnconfigure $f 0 -weight 1
  return $f.text
}


set in [open "showfile.txt" r+]
set file [gets $in]
close $in
set tx [Scrolled_Text .f -width 85 -height 30]
pack .f -side top -fill both -expand true
set in [open $file r+]
$tx insert end [read $in]
close $in

button .b1 -text "Dismiss" -width 23 \
  -command "exit"

pack .b1 -side top 


