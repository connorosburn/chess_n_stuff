import React from 'react';

function EndReport(props) {

    const report = (subject) => {
        if(subject) {
            return (
                    <p>{subject}</p>
            );
        }
    }

    const renderReport = () => {
        if(props.endState) {
            return (
                <div className="end-report">
                    {report(props.endState.condition)}
                    {report(props.endState.winner)}
                </div>
            );
        }
    }

    return renderReport();
}

export default EndReport;

