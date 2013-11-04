clear all
close all

fid = fopen('szakdoga_c/szakdoga/program/simulation/log/matlab.txt');

Fs = 48000;
matlab = 0;
dz = [];
dp = [];
cz = [];
cp = [];

while ~feof(fid)
    tline = fgetl(fid);
    if strcmp( tline, 'MATLAB>>>' )
        matlab = 1;
        break
    end
end

if matlab
    while ~feof(fid)
        tline = fgetl(fid);
        
        [tmp, cnt] = sscanf(tline, 'ID: %s', 1);
        if cnt
            simfile = ['P', tmp, '.txt'];
        end
        
        if strcmp( tline, '-- Transformed filter: -----------------------' )
             fgetl(fid);    % empty line
             tline = fgetl(fid);
             ca = sscanf(tline, 'Amplification: %g', 1);
             tline = fgetl(fid);
             if strcmp( tline(1:5), 'Zeros' )
                  tline = fgetl(fid);
                  k = 1;
                  [tmp, cnt] = sscanf(tline, '%g + %gi', 2);
                  while cnt
                      if( tmp(2) ~= 0 )
                          cz(k)     = tmp(1) + 1i*tmp(2);
                          cz(k+1)	= conj(cz(k));
                          k = k + 2;
                      else
                          cz(k)     = tmp(1);
                          k = k + 1;
                      end
                      fgetl(fid);
                      tline = fgetl(fid);
                      [tmp, cnt] = sscanf(tline, '%g + %gi', 2);
                  end
             end
             if strcmp( tline(1:5), 'Poles' )
                  tline = fgetl(fid);
                  k = 1;
                  [tmp, cnt] = sscanf(tline, '%g + %gi', 2);
                  while cnt
                      if( tmp(2) ~= 0 )
                          cp(k)     = tmp(1) + 1i*tmp(2);
                          cp(k+1)	= conj(cp(k));
                          k = k + 2;
                      else
                          cp(k)     = tmp(1);
                          k = k + 1;
                      end
                      fgetl(fid);
                      tline = fgetl(fid);
                      [tmp, cnt] = sscanf(tline, '%g + %gi', 2);
                  end
             end
             tline = fgetl(fid);
             if length(tline) > 10
                 if strcmp( tline(1:10), 'Number of ' )
                     tline = tline(11:end);
                 end
                 [tmp, cnt] = sscanf(tline, 'differentiators: %d', 1);
                 if cnt
                     cz = [cz zeros(1, tmp)];
                 else
                     [tmp, cnt] = sscanf(tline, 'integrators: %d', 1);
                     if cnt
                        cp = [cp zeros(1, tmp)];
                     else
                        [tmp, cnt] = sscanf(tline, 'j%g conjugate zero pairs: %d', 2);
                        if cnt == 2
                        	cz = [cz 1i*tmp(1)*ones(1, tmp(2)) conj(1i*tmp(1)*ones(1, tmp(2)))];
                        else
                            [tmp, cnt] = sscanf(tline, 'j%g conjugate pole pairs: %d', 2);
                            if cnt == 2
                                cp = [cp 1i*tmp(1)*ones(1, tmp(2)) conj(1i*tmp(1)*ones(1, tmp(2)))];
                            end
                        end
                     end
                 end
             end
        end
        
        if strcmp( tline, '-- Digital filter: ---------------------------' )
             fgetl(fid);    % empty line
             tline = fgetl(fid);
             da = sscanf(tline, 'Amplification: %g', 1);
             tline = fgetl(fid);
             if strcmp( tline(1:5), 'Zeros' )
                  tline = fgetl(fid);
                  k = 1;
                  [tmp, cnt] = sscanf(tline, '%g + %gi', 2);
                  while cnt
                      if( tmp(2) ~= 0 )
                          dz(k)     = tmp(1) + 1i*tmp(2);
                          dz(k+1)	= conj(dz(k));
                          k = k + 2;
                      else
                          dz(k)     = tmp(1);
                          k = k + 1;
                      end
                      tline = fgetl(fid);
                      [tmp, cnt] = sscanf(tline, '%g + %gi', 2);
                  end
             end
             if strcmp( tline(1:5), 'Poles' )
                  tline = fgetl(fid);
                  k = 1;
                  [tmp, cnt] = sscanf(tline, '%g + %gi', 2);
                  while cnt
                      if( tmp(2) ~= 0 )
                          dp(k)     = tmp(1) + 1i*tmp(2);
                          dp(k+1)	= conj(dp(k));
                          k = k + 2;
                      else
                          dp(k)     = tmp(1);
                          k = k + 1;
                      end
                      tline = fgetl(fid);
                      [tmp, cnt] = sscanf(tline, '%g + %gi', 2);
                  end
             end
              tline = fgetl(fid);
             if length(tline) > 10
                 if strcmp( tline(1:10), 'Number of ' )
                     tline = tline(11:end);
                 end
                 [tmp, cnt] = sscanf(tline, 'differentiators: %d', 1);
                 if cnt
                     dz = [dz ones(1, tmp)];
                 else
                     [tmp, cnt] = sscanf(tline, 'integrators: %d', 1);
                     if cnt
                        dp = [dp ones(1, tmp)];
                     else
                        [tmp, cnt] = sscanf(tline, 'e^j%g conjugate zero pairs: %d', 2);
                        if cnt == 2
                        	dz = [dz exp(1i * tmp(1))*ones(1, tmp(2)) conj(exp(1i * tmp(1))*ones(1, tmp(2)))];
                        else
                            [tmp, cnt] = sscanf(tline, 'e^j%g conjugate pole pairs: %d', 2);
                            if cnt == 2
                                dp = [dp exp(1i * tmp(1))*ones(1, tmp(2)) conj(exp(1i * tmp(1))*ones(1, tmp(2)))];
                            end
                        end
                     end
                 end
             end
        end
    end
    
    if strcmp( tline, '>>>MATLAB' )
        matlab = 0;
        break
    end
    
end

fclose(fid);

if length(dz) < length(dp)
    dz = [ dz -1*ones(1, length(dp)-length(dz)) ];
end

sysC = zpk(cz, cp, ca)
sysD = zpk(dz, dp, da, 1/Fs)

A = importdata(['szakdoga_c/szakdoga/program/simulation/output/', simfile], ' ', 10);
Q = A.data;
qA = Q(1,1);
Q = Q(:,2) / qA / 32768 / 2;

L = length(Q);
NFFT = 2^nextpow2(L); % Next power of 2 from length of y
Y = Fs * fft(Q,NFFT)/L;
Ylog = 20*log10(abs(Y(1:NFFT/2+1)))';
f = Fs/2*linspace(0,1,NFFT/2+1);

[isysD, t] = impulse(sysD, (L-1)/Fs);
isysD = isysD / Fs;

[isysC, t] = impulse(sysC, t);
isysC = isysC / Fs;

lim = max(isysC) * 1 / 100;
for k = 1:length(t)
    if abs(isysC(k)) > lim
        limk = k;
    end
end
lim = t(limk) * 2;

plot(t, isysC, t, isysD, t, Q)
xlim([0 lim])
xlabel('time [sec]')
ylabel('magnitude')
title('Impulse Response')
legend('Continous', 'Digital', 'Simulated')

[mag, phs] = bode(sysC, f*2*pi);
magC = 20*log10(mag(:,:));

[mag, phs] = bode(sysD, f*2*pi);
magD = 20*log10(mag(:,:));

% Plot single-sided amplitude spectrum.
figure
semilogx(f,magC, f,magD, f,Ylog, 'LineWidth',1)
axis([10 24500 -200 10])
grid on
xlabel('frequency [Hz]')
ylabel('magnitude [dB]')
title('Bode Plot')
legend('Continous', 'Digital', 'Simulated', 'Location', 'SouthWest')

irdAD = (isysC - isysD);
irdAS = (isysC - Q);
irdDS = (isysD - Q);
figure
subplot(2,1,1);
plot(t, irdAD, t, irdAS)
xlim([0 lim])
xlabel('time [sec]')
ylabel('difference')
title('Difference between impulse responses')
legend('C-D', 'C-S', 'Location', 'SouthEast')
subplot(2,1,2);
plot(t, irdDS, 'r')
xlim([0 lim])
xlabel('time [sec]')
ylabel('difference')
legend('D-S', 'Location', 'SouthEast')

bmdAD = (magC - magD);
bmdAS = (magC - Ylog);
bmdDS = (magD - Ylog);
figure
plot(f, bmdAD, f, bmdAS, f, bmdDS)
axis([10 24500 -100 100])
xlabel('frequency [Hz]')
ylabel('difference [dB]')
title('Difference between transfer characteristics')
legend('C-D', 'C-S', 'D-S', 'Location', 'SouthWest')
