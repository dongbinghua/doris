SET enable_nereids_planner=TRUE;
SET enable_fallback_to_original_planner=FALSE;
-- SELECT
--     label,
--     count() AS c
-- FROM github_events
-- LATERAL VIEW explode_split(labels, ',') t AS label
-- WHERE (event_type IN ('IssuesEvent', 'PullRequestEvent', 'IssueCommentEvent')) AND (action IN ('created', 'opened', 'labeled'))
-- GROUP BY label
-- ORDER BY c DESC
-- LIMIT 50
