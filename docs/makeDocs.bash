#!/bin/bash
latex2html Glue-Docs.tex -split 0 -dir html -mkdir --title 'RL-Glue 3.0' -local_icons -math
latex2html details.tex -split 0 -dir details_html -mkdir --title 'RL-Glue 3.0 Technical Details' -local_icons -math
#More advanced stuff to try if we run into probs
#latex2html c-codec.tex −split 0  --title 'RL-Glue C/C++ Codec' -dir html -mkdir -math -local_icons -html_version 4.0,math 
