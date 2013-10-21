close all

% files:
testroot = fullfile('szakdoga_c', 'szakdoga', 'program', 'simulation', 'output');
ffc = 'f_continous.txt';
ffd = 'f_discrete.txt';
fsi = "s_input.txt";
fso = "s_output.txt";

wmin = 2*pi*10;
wmax = 2*pi*25000;

Fs		= 48000;			% Sampling frequency
Ts		= 1/Fs;				% Sample time
Wlog	= logspace(1,log10(wmin),log10(wmax));

tc = chooseTest(testroot);

zpkC = readZpk(fullfile(testroot, tc.id, ffc));
zpkD = readZpk(fullfile(testroot, tc.id, ffd));

filterC = zpk(zpkC.z, zpkC.p, zpkC.k);
filterD = zpk(zpkD.z, zpkD.p, zpkD.k, Ts);

clear ffc, ffd, zpkC, zpkD

figure
bo = bodeoptions;
bo.Grid				= 'on';
bo.FreqUnits		= 'kHz';
bo.MagLowerLimMode	= 'manual';
bo.MagLowerLim		= -120;

bodeplot(filterC, filterD, Wlog, bo)

if tc.length ~= 0
	if tc.impulse ~= 0
		simIn = getSimInput(tc.impulse);
	else
		simIn = getSimInput(fullfile(testroot, tc.id, fsi));
	end
	
	simOut = getSimOutput(fullfile(testroot, tc.id, fso));
	t = 0:Ts:(tc.length-1);		% Time vector
	
	% normalize
	maxIn	= max(abs(simIn));
	simIn	= simIn / maxIn;
	simOut	= simOut / maxIn;

	% simulate
	[simC, t] = lsim(filterC, simIn, t);
	[simD, t] = lsim(filterD, simIn, t);

	% Plot simulation results
	figure
	plot( t,simIn, t,simOut, t,simC, t,simD )
	
	if tc.impulse ~= 0
		
		N = 2^nextpow2(tc.length);	% Next power of 2 from length of input
		Wlin = Fs/2*linspace(0, 1, N/2+1);

		Y = fft(sigOut,N)/L;

		[bodeC, phase] = bode(filterC, Wlin);
		[bodeD, phase] = bode(filterD, Wlin);

		clear phase

		% Plot single-sided amplitude spectrum
		figure
		semilogx( Wlin, 20*log10(abs(Y(1:N/2+1))) )
		hold on
		semilogx( Wlin, bodeC )
		semilogx( Wlin, bodeD )
		hold off

		clear bodeC, bodeD
	end
	
end

% ----------------------------------------
%  Select test
% ----------------------------------------
function test = chooseTest(dirname)
	list = dir(dirname);
	k = 0;
	for d = list
		if d.isdir == 1
			filename = fullfile(dirname, d.name, 'info.txt');
			fid = fopen(filename);
			if fid ~= -1
				tid = fscanf(fid, 'id: %s\n');
				if tid ~= d.name
					error('Corrupt info file:\n %s', filename);
				end
				[tname, c] = fscanf(fid, 'name: %s\n');
				if c ~= 1
					error('Corrupt info file:\n %s', filename);
				end
				[tdate, c] = fscanf(fid, 'date: %s %s\n');
				if c ~= 2
					error('Corrupt info file:\n %s', filename);
				end
				[tlength, c] = fscanf(fid, 'length: %u\n');
				if c ~= 1
					error('Corrupt info file:\n %s', filename);
				end
				[timpulse, c] = fscanf(fid, 'impulse: %i\n');
				if c ~= 1
					error('Corrupt info file:\n %s', filename);
				end
				fclose(fid);
				
				tdate = [tdate(1:10) ' ' tdate(11:end)];
				k = k + 1;
				tests(k) = struct('id', tid, 'name', tname, 'date', tdate, 'length', tlength, 'impulse', timpulse);
			end
		end
	end
	
	if k >= 1
		str = sprintf('Num Id         Date                Name       Impulse    Length\n') );
		%		 xxx xxxxxxxxxx	xxxx-xx-xx xx:xx:xx xxxxxxxxxx xxxxxxxxxx xxxx...
		disp(str);
		k = 1;
		for t = tests
			str = sprintf('%3d %10s %19s %-10s %-10d %d\n', k, test.id, test.date, test.name, test.impulse, test.length);
			disp(str);
			k = k + 1;
		end
		str = sprintf('Choose test! ( 1-%d for test, 0 to print summary to file, ENTER to cancel)', k-1);
		in = input(str);
		if in == 0
		elseif in > 0 && in < k
		else
			error('Invalid input value!');
		end
	else
		error('Testroot is empty!');
	end
	
end

% ----------------------------------------
%  Read ZPK informations from file
% ----------------------------------------
function zpk = readZpk(filename)
	fid = fopen(filename);
	
	if fid == -1
		error('Can''t open %s', filename);
	end
	
	[A, c] = fscanf(fid,'z:%u|p:%u\nk: %g\n');
	
	if c ~= 3
		error('Corrupt file: %s', filename);
	end
	
	z = A(1);
	p = A(2);
	zpk.k = A(3);
	
	if z > 0
		fscanf(fid,'zeros:\n');
		[A, c] = fscanf(fid,'%g+j%g\n');
		
		if c/2 ~= z
			fclose(fid);
			error('Corrupt zeros section in file: %s', filename);
		end
		
		kZ = 1;
		for kA = 1:z
			zpk.z(kZ) = A(1,kA) + A(2,kA) * 1i;
			kZ = kZ + 1;
			if A(2,kA) ~= 0
				zpk.z(kZ) = conj(zpk.z(kZ-1));
				kZ = kZ + 1;
			end
		end
		
	else
		zpk.z = [];
	end
	
	if p > 0
		fscanf(fid,'poles:\n');
		[A, c] = fscanf(fid,'%g+j%g\n');
		
		if c/2 ~= z
			fclose(fid);
			error('Corrupt poles section in file: %s', filename);
		end
		
		kZ = 1;
		for kA = 1:z
			zpk.p(kZ) = A(1,kA) + A(2,kA) * 1i;
			kZ = kZ + 1;
			if A(2,kA) ~= 0
				zpk.p(kZ) = conj(zpk.p(kZ-1));
				kZ = kZ + 1;
			end
		end
		
	else
		zpk.p = [];
	end
	
	fclose(fid);
end

% ----------------------------------------
%  Read simulation input from file
% or create impulse
% ----------------------------------------
function simIn = getSimInput(filename)

	if length(filename) > 1
		
		fid = fopen(filename);
		if fid == -1
			error("Can't open %s", filename);
		end

		[A, c] = fscanf(fid,"%i\n");
		if c ~= L
			error("Corrupt file: %s", filename);
		end

		simIn = A';
		clear A, c
		
	else

		simIn = fliplr(sparse( 1, L, filename ));
		
	end

end

% ----------------------------------------
%  Read simulation output from file
% ----------------------------------------
function simOut = getSimOutput(filename, length)
	filename = fso;
	fid = fopen(filename);
	if fid == -1
		error("Can't open %s", filename);
	end

	[A, c] = fscanf(fid,"%i\n");
	if c ~= length
		error("Corrupt file: %s", filename);
	end

	simOut = A';
end