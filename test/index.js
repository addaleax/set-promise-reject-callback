'use strict';

const promiseReject = require('../');
const assert = require('assert');

describe('set-promise-reject-callback', function() {
  const events = [];
  beforeEach(function() {
    events.splice(0);

    promiseReject.SetCallback((event, promise, reason) => {
      events.push({event, promise, reason});
    });
  });

  it('tracks promises', function() {
    events.push('start');
    const a = Promise.reject(42);
    events.push('checkpoint 1');
    a.catch(() => {});
    events.push('end');
    assert.deepStrictEqual(events, [
      'start',
      { event: 0, promise: a, reason: 42 },
      'checkpoint 1',
      { event: 1, promise: a, reason: undefined },
      'end'
    ]);
  });
});
