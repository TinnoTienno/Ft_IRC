Legende : 
 # = categorie de test
 / = test valide
 () = commentaires

 !!! PRIVMSG vide dans bot
# connect

connect
/connect with same nick/hostname

# authentification
/nc + authentification

#invite
/nc invite
/invite ""
/invite noda #cha
/invite wronguser #cha
/invite noda #wrongchan
/invite noda

# join
/nc join 
/join ""
/join chan
/join #chan
/join #chan
/join ,#chan
/join #chan mdp + join #chan mdp
/join #chan mdp + join #chan mdps
/join #chan1,#chan2
/join #chan1,#chan2 mdp1,mdp2 + /join #chan1,#chan2 mdp1,mdp2
/join #cha + /join #Cha

# kick
/nc kick
/kick ""
/kick noda
/kick #chan
/kick #chan wronguser
/kick wronguser
/kick #cha self

# mode Arthur
(A la fin, je suis pas fou)

/Modifier sendAllMode en premier

/nc mode
mode ""
/mode #chan ""
/mode #wrongchan ""
/mode #chan
/mode #closedchan b
/mode #unclosedchan b
/mode noda +o
/mode #chan +o ""
/mode #chan -o ""
/mode #chan -o op
/mode #chan +t
/mode #chan +l ""
/mode #chan +k ""
/mode #chan b ""
/mode #chan +b ""
/mode #chan -b ""
/mode #chan b (OP)
/mode #chan b (Not OP)
/mode #chan +o user2
/mode #chan +o noda (Already op)
/mode #chan +o

# nick
/nc nick
/nick ""
/nick noda + nick noda
/nick #chan
/Tested at connection too

# notice

/nc notice
/notice "" coucou
/notice "" ""
/notice #chan ""
/notice #chan 
/notice #chan coucou
/notice noda coucou
/join #chan mdp + notice #chan coucou (Looks off : #chan Cannot send to channel || :bess-f4r3s11.clusters.42paris.fr 404 noda_ #chan :Cannot send to channel)
/notice wrongusername coucou
/notice #wrongchannel coucou

# part

/nc part
/part ""
/part #chan
/part #wrongchan
/part existingchan
/part noda

# ping
/nc ping
/ping ""

# privmsg

/nc privmsg 
/privmsg "" coucou
/privmsg "" ""
/privmsg #chan "" (Same as notice)
/privmsg #chan coucou
/privmsg noda coucou
/mode #chan +i + privmsg #chan coucou
/privmsg wrongusername coucou

# quit
/quit
/join #newchan + quit
 
# topic Aduvilla
nc topic 
/topic #chan
/topic #chan voici le topic 
/mode +t   + topic #chan voici le topic

# bot

/mode #chan +i  + bot #chan -> Bot must quit when he cant join channel
Proteger quand le nickname est deja pris ? Ca va pas poser de probleme ca 

# Other tests

/nc mode + Ctrl D
