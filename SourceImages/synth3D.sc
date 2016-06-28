(
SynthDef("simpleSine", {
	arg freq = 440, mul = 0, detune=1.01;
	var out;
	out = SinOsc.ar([freq, freq*detune], pi.rand2, mul);
	Out.ar(3, out);
}).store;

SynthDef("fx", {
	var out;
	out = In.ar(3, 2);
	//2.do({ out = AllpassL.ar(out, 0.1, LFNoise2.kr([rrand(0.0, 0.1),rrand(0.0, 0.1)],0.01,0.06), 1.0) });
	//8.do { out = AllpassL.ar(out, 0.04, 0.04.rand, 2) };
	out = LeakDC.ar(out);
	/*
	out = CompanderD.ar(out, out,
		thresh:0.1,
		slopeBelow: 1,
		slopeAbove: 0.1,
		clampTime: 0.01,
		relaxTime: 0.01,
		mul: 1.0);
	out = GVerb.ar(out,roomsize: 99, revtime: 1);
	*/
	Out.ar(0, out);
}).store;

SynthDef("col_closesaw", {
	arg fadeTime = 10, n = 0, rq = 0.3, detune = 0.001, base = 20, ratio = 1.5, harm = 1.5, amp = 0.2, gate=0;
	var lfo, env, out;
	env = EnvGen.kr(Env.new([0,1], [fadeTime], 'sine'));
	lfo = SinOsc.ar(rrand(0.03, 0.05), 0, 100, 600);
	out = Saw.ar([base+detune.rand, base+detune.rand] * (ratio ** n)) * amp
	+ Saw.ar([base*harm+detune.rand, base*harm+detune.rand] * (ratio ** n)) * amp;
	out = out * env;
	out = RLPF.ar(out, lfo * (1.5 ** n), rq).clip2 * 0.5;
	out = out * EnvGen.kr(Env.adsr(releaseTime:10), gate, doneAction: 2);
	Out.ar(5, out);
}).store;
/*
SynthDef("col_closefx", {
	arg lpf=440, rq=0.5, amp=0.8;
	var in, out;
	in = In.ar(5, 2);
	32.do({ in = AllpassL.ar(in, 0.1, LFNoise2.kr([rrand(0.0, 0.1),rrand(0.0, 0.1)],0.01,0.06), 2.0) });
	//out = Limiter.ar(MidEQ.ar(in, freq, 0.5, 8));
	out = CompanderD.ar(in) * amp;
	//out = Limiter.ar(in) * amp;
	//out = CompanderD.ar(MidEQ.ar(in, freq, 0.7, 8)) * amp;
	Out.ar(0, out);
}).store;
*/

SynthDef("col_closefx", {
	arg lpf=440, rq=0.5, amp=0.8;
	var in, out;
	in = In.ar(5, 2);
	32.do({ in = AllpassL.ar(in, 0.1, LFNoise2.kr([rrand(0.0, 0.1),rrand(0.0, 0.1)],0.01,0.06), 0.5) });
	out = CompanderD.ar(in) * amp;
	Out.ar(0, out);
	//Out.ar(7, out);
}).store;

SynthDef("col_comb", {
	arg amp = 0.0;
	var out;
	out = In.ar(5, 2) + In.ar(3, 2);
	out = CombL.ar(out, 1.0,  MouseX.kr(0.005, 0.1), 20);
	//in = Pan2.ar(in, LFNoise0.kr(1));
	//out = out * amp;
	out = Limiter.ar(out) * amp;
	Out.ar(0, out);
}).store;
)
