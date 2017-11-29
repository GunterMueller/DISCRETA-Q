wm title . "Show"


listbox .dateien -relief raised -borderwidth 2 -width 60 -yscrollcommand ".scroll set"
pack .dateien -side left
scrollbar .scroll -command ".dateien yview"
pack .scroll -side right -fill y
foreach i [lsort [glob *]] {
  .dateien insert end $i
}


