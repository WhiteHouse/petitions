/*
 * Helper function to add commas to large numbers like the response threshold.
 */
function addCommas(number) {
    'use strict';
    var l, out, n, i;
    number = number.toString();
    l = number.length;
    out = '';
    n = 0;
    for (i = (l - 1); i >= 0; i--) {
        out = '<span class="ln">' + number.charAt(i) + '</span>' + out;
        if ((l - i) % 3 === 0 && i !== 0) {
            out = '<span class="lcom">,</span>' + out;
        }
        n++;
    }
    return out;
}

/*
 * Animates the total signatures count.
 */
function animateText(el, fromValue, toValue) {
    'use strict';
    var total, interval;
    total = fromValue;
    interval = setInterval(function () {
        if (total < toValue) {
            // 2000ms for the animation, we update every 50ms
            total += parseInt(((toValue - fromValue) / (2000 / 50)), null);
            total = Math.min(total, toValue);
            el.html(addCommas(total));
        } else {
            clearInterval(interval);
        }
    }, 50);
    return interval;
}

/*
 * Animates the thermometer.
 */
function animateThermometer(valueEl, toHeight, totalEl, totalValue, callback) {
    'use strict';
    valueEl.animate({'height': toHeight}, 2000, function () {
        totalEl.html(addCommas(totalValue));
        callback();
    });
}

/*
 * Helper function that sets variables used during animation.
 */
function animateValues(valueEl, totalEl, fromValue, toValue, goalValue, pixelsPerValue) {
    'use strict';
    var toHeight, interval;
    toHeight = Math.min(pixelsPerValue * toValue, pixelsPerValue * goalValue);
    interval = animateText(totalEl, fromValue, toValue);
    animateThermometer(valueEl, toHeight, totalEl, toValue,
        function () {
            clearInterval(interval);
        });
    return interval;
}

/*
 * Adds all of the thermometer mark highlights.
 */
function addMarkHighlights(middleEl) {
    'use strict';
    var el;
    middleEl.find('.mark').hover(function () {
        el = $(this);
        el.addClass('mark-selected');
    },
        function () {
            el = $(this);
            el.removeClass('mark-selected');
        });
}

/*
 * Adds all of the thermometer marks.
 */
function addThermometerMarks(middleEl, numberOfMarks, valuePerMark) {
    'use strict';
    var i, amount, markEl, tooltip;
    for (i = 1; i <= numberOfMarks; ++i) {
        amount = parseInt((valuePerMark * i), null);
        markEl = $('<div class="mark"></div>');
        markEl.css({'position': 'absolute', 'bottom': (i * 10) + "px"});
        markEl.attr('title' + amount);
        tooltip = $('<div class="tooltip">' + amount + '</div>');
        markEl.append(tooltip);
        middleEl.append(markEl);
    }
}

/*
 * Writes static petition data to the HTML document.
 */
function writeData(link, created, issues, total, needed, date, threshold, responseurl, responselink) {
    'use strict';
    var i, issue;
    $("#petition-title").html(link);
    $("#created").html(created);
    for (i = 0; i < issues.length; i++) {
        issue = "";
        issue += issues[i].name;
        if (i !== issues.length - 1) {
            issue += ", ";
        }
        $("#issues").append(issue);
    }
    $("#total-signatures").html(addCommas(total));
    $("#remaining-signatures").html(addCommas(needed));
    $("#response-deadline").html(date);
    $("#response-threshold").html(addCommas(threshold));
    if (responseurl) {
        $("#response-link").html(responselink);
    }
}

/*
 * Assigns variables to all of the data returned by JSONP then calls write and
 * animation functions.
 */
function petitionData(data) {
    'use strict';
    // Define variables with data returned via JSON.
    var petitionTitle, petitionUrl, petitionLink, petitionDate, petitionCreated,
        petitionIssues, signaturesTotal, signaturesNeeded, petitionDeadline,
        deadlineDate, responseThreshold, responseUrl, responseLink, el,
        middleEl, middleValueEl, numberOfMarks, valuePerMark, pixelsPerValue,
        signaturesTotalEl;
    petitionTitle = data.results[0].title;
    petitionUrl = data.results[0].url;
    petitionLink = '<a href="' + petitionUrl + '" target="_blank">' + petitionTitle + '</a>';
    petitionDate = new Date(data.results[0].created * 1000);
    petitionCreated = dateFormat(petitionDate, "mmm dd, yyyy");
    petitionIssues = data.results[0].issues;
    signaturesTotal = data.results[0]['signatureCount'];
    signaturesNeeded = data.results[0]['signaturesNeeded'];
    petitionDeadline = new Date(data.results[0].deadline * 1000);
    deadlineDate = dateFormat(petitionDeadline, "mmm dd, yyyy");
    responseThreshold = data.results[0]['signatureThreshold'];
    if (data.results[0].response !== null) {
        responseUrl = data.results[0].response.url;
    }
    responseLink = '<a href="' + responseUrl + '" target="_blank">View Petition Response</a>';

    // Define additional variables used in animation.
    el = $("#thermometer-widget");
    middleEl = el.find('#middle');
    middleValueEl = middleEl.find('.value');
    numberOfMarks = parseInt(middleEl.height() / 10, 10);
    valuePerMark = responseThreshold / numberOfMarks;
    pixelsPerValue = middleEl.height() / responseThreshold;
    signaturesTotalEl = el.find('.display-total-signatures');

    // Execute functions that populate and animate the thermoeter widget.
    writeData(petitionLink, petitionCreated, petitionIssues, signaturesTotal, signaturesNeeded, deadlineDate, responseThreshold, responseUrl, responseLink);
    addThermometerMarks(middleEl, numberOfMarks, valuePerMark);
    addMarkHighlights(middleEl);
    animateValues(middleValueEl, signaturesTotalEl, 0, signaturesTotal, responseThreshold, pixelsPerValue);
}
