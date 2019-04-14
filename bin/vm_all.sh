list=(
# 42.cor						# big
# barriere.cor					# big
Asombra.cor						# 30360 OK
Bazou.cor						# 59490 OK
Code_eater.cor					# 240335 OK
Eraser.cor						# 3071 OK
Explosive_Kitty.cor				# 39217 OK
Gagnant.cor						# 26023 OK
Hidden.cor						# 27438 OK
Horace.cor						# 59837 OK
Izi_Miaou.cor					# 239777 OK
Kappa.cor						# 25902 OK
Kitty_Miaou.cor					# 239777 OK
Kittystrophic.cor				# 116171 OK
Machine-gun.cor					# 3071 OK
MarineKing.cor					# 27438 OK
Misaka_Mikoto.cor				# 27848 OK
NoIdea.cor						# 25541 OK
O-Maley_Miaou.cor				# 239777 OK
Rainbow_dash.cor				# 83333 OK
Stargate0_1.cor					# 29109 OK
THUNDER.cor						# 52680 OK
TheHarvester.cor				# 3071 OK
Varimathras.cor					# 76223 OK
Wall.cor						# 27438 OK
_.cor							# 27438 OK
_honeybadger.cor				# 1535 OK
bigzork.cor						# 28362 OK
ex.cor							# 3071 OK
fluttershy.cor					# 25902 OK
jumper.cor						# 24690 OK
mortel.cor						# 7429 OK
toto.cor						# 26023 OK
turtle.cor						# 40480 OK
casimir.cor						# 28151 OK
champ.cor						# 241759 OK
corelol.cor						# 57954 OK
darksasuke.cor					# 32022 OK
doge.cor						# 26226 OK
dubo.cor						# 3071 OK
echo.cor						# 3071 OK
fast.cor						# 13173 OK
gateau.cor						# 29009 OK
gedeon.cor						# 25902 OK
zork.cor						# 57954 OK
justin_bee.cor					# 27000 OK
kamelkaze.cor					# 3071 OK
katchup.cor						# 127415 OK
lapsang.cor						# 31697 OK
le_pet_de_nonne.cor				# 3071 OK
live.cor						# 20439 OK
loose.cor						# 29237 OK
lou_forktologue.cor				# 27000 OK
littlepuppy.cor					# 3071 OK
master_of_puppets.cor			# 29760 OK
maxidef.cor						# 25901 OK
salamahenagalabadoun.cor		# 3071 OK
overwatch.cor					# 27438 OK
pai_mu_tan.cor					# 239963 OK
run_Kitty_RUN.cor				# 127189 OK
sam_2.0.cor						# 111225 OK
sencha.cor						# 239777 OK
skynet.cor						# 25902 OK
slider2.cor						# 25902 OK
tchupka.cor						# 127415 OK
terminator.cor					# 22755 OK
ultima.cor						# 28224 OK
ultimate-surrender.cor			# 3071 OK
# Car.cor						# very long
# mandragore.cor				# very long
# jinx.cor						# 24366 OK
# youforkmytralala.cor			# 24827 OK
# big_feet.cor					# 25901 OK
# wave.cor						# 27438 OK
# meowluigi.cor					# 27438 OK
# mise_a_jour_windows95.cor		# 27438 OK
# Douceur_power.cor				# 27438 OK
# ForgottenOne.cor				# 27438 OK
# Octobre_Rouge_V4.2.cor		# 27438 OK
# helltrain.cor					# 27438 OK
# bee_gees.cor					# 27438 OK
)

for i in ${list[@]}; do
	echo "\t"$i
	sh bin/vm_bool.sh resources/champs/cor/$i
done;
