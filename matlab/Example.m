readChannelID = [351435];
% TODO - Enter the Read API Key between the '' below:
readAPIKey = 'FL3XZIDD8M8D19KR';

% TODO - Replace the [] with channel ID to write data to:
writeChannelID = [];
% TODO - Enter the Write API Key between the '' below:
writeAPIKey = '';

%% Read Data %%
data = thingSpeakRead(readChannelID, 'ReadKey', readAPIKey, 'Fields',1,'NumPoints',100,'outputFormat','timetable');

% Print out raw table
data

%% Filter Data %%
%S = timerange('12/18/2015 08:00:00','12/18/2015 12:00:00');
%31
%'yyyy-mm-dd HH:MM:SS'
%2000-03-01 15:45:17
%%
S = timerange(datestr('2017-10-24 11:10:00', 31), datetime('now'));
%S = timerange(datestr('2017-10-24 16:10:00', 31), datestr('2017-10-24 17:10:00', 31));
filteredData = data(S,:);
%% NOTE: Must use UTC time

filteredData

%% FORM STUDENT KEYS
% read in student and tag associations into a table
T = readtable('students.csv');

% Address table by the tag IDs
tagIDs = table2cell(T(:, 3));
T.Properties.RowNames = tagIDs;

% Add column called "Present" and set to zero
T.Present = zeros(height(T), 1);
% Add a columnt called "Date" and put today's date in
T.Date = repmat({datestr(today)}, height(T), 1); 

% For every filtered tag ID we've found, mark the student present
%  This is accomplished by setting the 'Present' field to 1
for i = 1:length(filteredData.TagEPC)
    epc = filteredData.TagEPC{i};
    T({epc}, :).Present = 1;
end

% Make a copy but only save Last name, First name, Present and Date
Tout = T(:, {'LastName', 'FirstName', 'Present', 'Date'});

% Write this output to a file [Note: Could also use .xlsx to open in Excel
writetable(Tout, 'out.xlsx')
