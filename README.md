# SoundEffects
A tiny program realized sound fade-in, fade-out, fade-away

make sure the audio file you want to change is in the same directory as the executable "effect"

Type in shell:

xxd -g1 crow.wav | ./effects -fout 2000 | xxd -r > crow_short_fade.wav

OR

xxd -g1 crow.wav | ./effects -fin 2000 | xxd -r > crow_short_fade.wav

OR

xxd -g1 crow.wav | ./effects -pan 2000 | xxd -r > crow_short_fade.wav

to yield a modified audio "crow_short_fade"
